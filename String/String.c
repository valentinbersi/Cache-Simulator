#include "String.h"
#include <string.h>
#include <malloc.h>
#include <math.h>

#define BLANK ""
#define UNSIGNED_FORMAT "%u"
#define HEX_FORMAT "%x"
#define END_CHARACTER_STRING "\0"
#define END_CHARACTER '\0'
#define DELIMITER_POSITION 0

static const int END_CHARACTER_SIZE = 1;

static const u_int8_t EQUAL = 0;

void stringConcatString(char **dst, const char *const src) {
    if (dst == NULL || (*dst == NULL && src == NULL)) return;

    size_t dstSize = 0, srcSize = 0;
    char *auxString = NULL;

    if (*dst != NULL) {
        dstSize = strlen(*dst);
        auxString = strdup(*dst);
        free(*dst);
    }

    if (src != NULL) srcSize = strlen(src);

    *dst = malloc(dstSize + srcSize + 1);

    for (size_t i = 0; i < dstSize; ++i) (*dst)[i] = auxString[i];
    free(auxString);

    for (size_t i = 0; i < srcSize; ++i) (*dst)[i + dstSize] = src[i];

    (*dst)[dstSize + srcSize] = END_CHARACTER;
}

/**
 * Returns the number of digits of the given number in the given base
 * @param number The number
 * @param base The base
 * @return The number of digits of the given number in the given base
*/
u_int8_t numberOfDigits(const u_int32_t number, const NumericBase_e base) {
    return number == 0 ? 1 : (u_int8_t) (log2((double) number) / log2((double) base)) + 1; // Scary math
}

/**
 * Returns a malloc string with the representation of the given number in the given base.
 * @param number The number to be parsed
 * @param base The base in which the number is going to be parsed
 * @return The string representation of the given number in the given base.
 */
char *unsignedToString(const u_int32_t number, const NumericBase_e base) {
    u_int8_t numSize = numberOfDigits(number, base);
    char *newString = malloc(numSize + END_CHARACTER_SIZE);

    base == DEC ? sprintf(newString, UNSIGNED_FORMAT, number) : sprintf(newString, HEX_FORMAT, number);
    return newString;
}

/**
 * Counts the elements of the splitFunction if it was delimited by delimiter
 * @param string The string that will be split
 * @param delimiter The delimiter
 * @return The number of elements the string has
 */
size_t countElements(const char *string, const char delimiter) {
    if (string == NULL) return 0;

    bool wasDelimiter = false;
    size_t counter = 0;


    for (int i = 0; string[i] != END_CHARACTER; ++i) {
        if (string[i] == delimiter && !wasDelimiter) {
            wasDelimiter = true;
            counter++;
        } else {
            wasDelimiter = false;
        }
    }

    return counter + 1;
}

char **split(char *string, char *delimiter) {
    size_t wordsAmount = countElements(string, delimiter[DELIMITER_POSITION]);
    char **splittedValues = (char **) malloc(sizeof(char *) * wordsAmount);
    size_t iterator = 0;
    while (string) {
        char *splitedValue = strsep(&string, delimiter);
        while (strcmp(splitedValue, BLANK) == EQUAL && iterator < wordsAmount) {
            splitedValue = strsep(&string, delimiter);
        }
        if (strcmp(splitedValue, END_CHARACTER_STRING) != EQUAL) {
            splittedValues[iterator] = splitedValue;
            iterator++;
        }

    }

    return splittedValues;
}

void stringConcatUnsigned(char **dst, const u_int32_t number, const NumericBase_e base) {
    char *stringNumber = unsignedToString(number, base);
    stringConcatString(dst, stringNumber);
    free(stringNumber);
}
