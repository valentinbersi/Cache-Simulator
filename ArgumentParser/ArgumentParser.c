#include "ArgumentParser.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../String/String.h"

#define FILE 1
#define SIZE 2
#define WAYS 3
#define SETS 4
#define VERBOSE 5
#define MIN_INSTRUCTION 6
#define MAX_INSTRUCTION 7

static const u_int8_t NUMBER_BASE = 10;
static const u_int8_t EQUAL = 0;
static const u_int8_t NO_VERBOSE_ARGS = 5;
static const u_int8_t VERBOSE_ARGS = 8;
static const u_int8_t ADDRESS_BITS = 32;

const char *NO_ERROR = "";
const char *ARGUMENTS_NOT_VALID = "Arguments not valid";

const char *NOT_NUMERIC_SIZE_ARGUMENT = "Size argument must be a number";
const char *LOWER_OR_EQUAL_THAN_0_SIZE_ARGUMENT = "Size argument must be greater than 0";
const char *NOT_POWER_OF_TWO_SIZE_ARGUMENT = "Size argument must be a power of two";

const char *NOT_NUMERIC_LINE_ARGUMENT = "Line argument must be a number";
const char *LOWER_OR_EQUAL_THAN_0_LINE_ARGUMENT = "Line argument must be greater than 0";
const char *NOT_POWER_OF_TWO_LINE_ARGUMENT = "Line argument must be a power of two";

const char *NOT_NUMERIC_SETS_ARGUMENT = "Sets argument must be a number";
const char *LOWER_OR_EQUAL_THAN_0_SETS_ARGUMENT = "Sets argument must be greater than 0";
const char *NOT_POWER_OF_TWO_SETS_ARGUMENT = "Sets argument must be a power of two";

const char *INCOMPATIBLE_ARGUMENTS = "The amount of sets and lines is to big compared to the size of the cache";

const char *VERBOSE_FLAG = "-v";

const char *BAD_VERBOSE_FLAG = "Verbose flag not recognized";

const char *NOT_NUMERIC_MIN_VERBOSE_ARGUMENT = "Min verbose argument must be a number";
const char *LOWER_THAN_0_MIN_VERBOSE_ARGUMENT = "Min verbose argument must be greater or equal than 0";

const char *NOT_NUMERIC_MAX_VERBOSE_ARGUMENT = "Max verbose argument must be a number";
const char *LOWER_THAN_0_MAX_VERBOSE_ARGUMENT = "Max verbose argument must be greater or equal than 0";

const char *MIN_VERBOSE_GREATER_THAN_MAX = "Min verbose operation must be less than max verbose operation";

const char *WRONG_ARGUMENT_NUMBER = "The arguments number is wrong";

const char *POINT = ".";
const char *SLASH = "/";
const u_int8_t POINT_CHAR = 0;
const u_int8_t SLASH_CHAR = 1;

const char *HYPHEN = "-";
const char *UNDERSCORE = "_";

const char *OUTPUT_FILE_EXTENSION = ".txt";

struct Arguments {
    char *fileName;
    u_int32_t cacheSize;
    u_int32_t cacheWays;
    u_int32_t cacheSets;
    bool isVerbose;
    u_int32_t minVerboseOperation;
    u_int32_t maxVerboseOperation;
    const char *errorMessage;
};

/**
 * @return A malloc allocated Arguments struct with all its attributes set to 0 or NULL
 */
Arguments_t *initializeArguments(void) {
    Arguments_t *arguments = (Arguments_t *) malloc(sizeof(Arguments_t));
    arguments->fileName = NULL;
    arguments->cacheSize = 0;
    arguments->cacheWays = 0;
    arguments->cacheSets = 0;
    arguments->isVerbose = false;
    arguments->minVerboseOperation = 0;
    arguments->maxVerboseOperation = 0;
    arguments->errorMessage = NULL;
    return arguments;
}

/**
 * @param argc The argument count
 * @return True if the argument count is either NO_VERBOSE_ARGS or VERBOSE_ARGS
 */
bool isArgumentAmountValid(const int argc) {
    return argc != NO_VERBOSE_ARGS && argc != VERBOSE_ARGS;
}

/**
 * Stores the file argument in the Arguments_t struct.
 * @param arguments The arguments struct.
 * @param argv The arguments count
 * @return An error message if an error occurs during parsing. If there's no errors, returns NO_ERROR.
 */
const char *parseFileArgument(Arguments_t *const arguments, char **argv) {
    if (argv == NULL || arguments == NULL || argv[FILE] == NULL) return ARGUMENTS_NOT_VALID;

    arguments->fileName = strdup(argv[FILE]);

    return NO_ERROR;
}

/**
 * @param n The number to be checked
 * @return True if the number is power of two
 */
bool isPowerOfTwo(const int64_t n) {
    if (n < 0) return false;

    return (ceil(log2((double) n)) == floor(log2((double) n)));
}

/**
 * Stores the size, lines and set arguments in the Arguments_t struct.
 * @param arguments The arguments struct
 * @param argv The arguments count
 * @return An error message if an error occurs during parsing. If there's no errors, returns NO_ERROR.
 */
const char *parseSizeArguments(Arguments_t *const arguments, char **argv) {
    if (arguments == NULL || argv == NULL || argv[SIZE] == NULL || argv[WAYS] == NULL || argv[SETS] == NULL)
        return ARGUMENTS_NOT_VALID;

    char *conversionErrorHandler;
    int64_t aux;

    aux = strtol(argv[SIZE], &conversionErrorHandler, NUMBER_BASE);
    if (strcmp(conversionErrorHandler, NO_ERROR) != EQUAL) return NOT_NUMERIC_SIZE_ARGUMENT;
    if (aux <= 0) return LOWER_OR_EQUAL_THAN_0_SIZE_ARGUMENT;
    if (!isPowerOfTwo(aux)) return NOT_POWER_OF_TWO_SIZE_ARGUMENT;
    arguments->cacheSize = aux;

    aux = strtol(argv[WAYS], &conversionErrorHandler, NUMBER_BASE);
    if (strcmp(conversionErrorHandler, NO_ERROR) != EQUAL) return NOT_NUMERIC_LINE_ARGUMENT;
    if (aux <= 0) return LOWER_OR_EQUAL_THAN_0_LINE_ARGUMENT;
    if (!isPowerOfTwo(aux)) return NOT_POWER_OF_TWO_LINE_ARGUMENT;
    arguments->cacheWays = aux;

    aux = strtol(argv[SETS], &conversionErrorHandler, NUMBER_BASE);
    if (strcmp(conversionErrorHandler, NO_ERROR) != EQUAL) return NOT_NUMERIC_SETS_ARGUMENT;
    if (aux <= 0) return LOWER_OR_EQUAL_THAN_0_SETS_ARGUMENT;
    if (!isPowerOfTwo(aux)) return NOT_POWER_OF_TWO_SETS_ARGUMENT;
    arguments->cacheSets = aux;

    if ((log2(arguments->cacheSize / (arguments->cacheSets * arguments->cacheWays)) + log2(arguments->cacheSets))
        >= ADDRESS_BITS)
        return INCOMPATIBLE_ARGUMENTS;

    return NO_ERROR;
}

/**
 * Stores the verbose flag arguments in the Arguments_t struct.
 * @param arguments The arguments struct
 * @param argv The arguments array
 * @param argc The arguments count
 * @return An error message if an error occurs during parsing. If there's no errors, returns NO_ERROR.
 */
const char *parseVerboseArguments(Arguments_t *const restrict arguments,
                                  int argv,
                                  char **argc) {

    if (arguments == NULL || argc == NULL || argc[SIZE] == NULL || argc[WAYS] == NULL || argc[SETS] == NULL)
        return ARGUMENTS_NOT_VALID;

    if (argv < VERBOSE_ARGS) return NO_ERROR;
    if (strcmp(argc[VERBOSE], VERBOSE_FLAG) != EQUAL) return BAD_VERBOSE_FLAG;

    char *conversionErrorHandler;
    int64_t aux;

    arguments->isVerbose = true;

    aux = strtol(argc[MIN_INSTRUCTION], &conversionErrorHandler, NUMBER_BASE);
    if (strcmp(conversionErrorHandler, NO_ERROR) != EQUAL) return NOT_NUMERIC_MIN_VERBOSE_ARGUMENT;
    if (aux < 0) return LOWER_THAN_0_MIN_VERBOSE_ARGUMENT;
    arguments->minVerboseOperation = (u_int32_t) aux;

    aux = strtol(argc[MAX_INSTRUCTION], &conversionErrorHandler, NUMBER_BASE);
    if (strcmp(conversionErrorHandler, NO_ERROR) != EQUAL) return NOT_NUMERIC_MAX_VERBOSE_ARGUMENT;
    if (aux < 0) return LOWER_THAN_0_MAX_VERBOSE_ARGUMENT;
    arguments->maxVerboseOperation = (u_int32_t) aux;

    if (arguments->minVerboseOperation > arguments->maxVerboseOperation) return MIN_VERBOSE_GREATER_THAN_MAX;

    return NO_ERROR;
}

Arguments_t *parseArguments(int argc, char **argv) {
    Arguments_t *arguments = initializeArguments();

    if (isArgumentAmountValid(argc)) {
        arguments->errorMessage = WRONG_ARGUMENT_NUMBER;
        return arguments;
    }

    arguments->errorMessage = parseFileArgument(arguments, argv);
    if (strcmp(arguments->errorMessage, NO_ERROR) != EQUAL) return arguments;

    arguments->errorMessage = parseSizeArguments(arguments, argv);
    if (strcmp(arguments->errorMessage, NO_ERROR) != EQUAL) return arguments;

    arguments->errorMessage = parseVerboseArguments(arguments, argc, argv);

    return arguments;
}

char *argumentsGetFileName(const Arguments_t *const restrict arguments) {
    if (arguments == NULL) return NULL;
    return arguments->fileName;
}

u_int32_t argumentsGetCacheSize(const Arguments_t *const restrict arguments) {
    if (arguments == NULL) return 0;
    return arguments->cacheSize;
}

u_int32_t argumentsGetLines(const Arguments_t *const restrict arguments) {
    if (arguments == NULL) return 0;
    return arguments->cacheWays;
}

u_int32_t argumentsGetCacheSets(const Arguments_t *const restrict arguments) {
    if (arguments == NULL) return 0;
    return arguments->cacheSets;
}

bool argumentsIsVerbose(const Arguments_t *const restrict arguments) {
    if (arguments == NULL) return false;
    return arguments->isVerbose;
}

u_int32_t argumentsGetMinVerboseOperation(const Arguments_t *const restrict arguments) {
    if (arguments == NULL) return 0;
    return arguments->minVerboseOperation;
}

u_int32_t argumentsGetMaxVerboseOperation(const Arguments_t *const restrict arguments) {
    if (arguments == NULL) return 0;
    return arguments->maxVerboseOperation;
}

const char *argumentsGetErrorMessage(const Arguments_t *const restrict arguments) {
    if (arguments == NULL) return NULL;
    return arguments->errorMessage;
}

/**
 * @param inputFile The input file name
 * @return True if the input file is requiring to search the file in the current folder
 */
bool isIndicatingCurrentFolder(const char *const restrict inputFile) {
    return inputFile[POINT_CHAR] == *POINT && inputFile[SLASH_CHAR] == *SLASH;
}

char *argumentsGetOutputFile(const Arguments_t *const restrict arguments) {
    char *inputFile = (char *) argumentsGetFileName(arguments);

    if (inputFile == NULL) return NULL;

    if (isIndicatingCurrentFolder(inputFile)) {
        strsep(&inputFile, POINT);
        strsep(&inputFile, SLASH);
    }

    char *outputFile = NULL;
    stringConcatString(&outputFile, strsep(&inputFile, POINT));
    stringConcatString(&outputFile, UNDERSCORE);
    stringConcatUnsigned(&outputFile, argumentsGetCacheSize(arguments), DEC);
    stringConcatString(&outputFile, HYPHEN);
    stringConcatUnsigned(&outputFile, argumentsGetLines(arguments), DEC);
    stringConcatString(&outputFile, HYPHEN);
    stringConcatUnsigned(&outputFile, argumentsGetCacheSets(arguments), DEC);
    stringConcatString(&outputFile, OUTPUT_FILE_EXTENSION);

    return outputFile;
}

void argumentsFree(Arguments_t *const restrict arguments) {
    free(arguments->fileName);
    free(arguments);
}
