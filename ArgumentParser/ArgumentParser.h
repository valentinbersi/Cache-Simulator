#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <sys/types.h>
#include <stdbool.h>

typedef struct Arguments Arguments_t;

/**
 * Parses the given arguments, and returns them in a Arguments struct. In the Arguments struct also comes an error message, if it´s empty then no error was detected during parsing.
 * @param argc The amount of arguments
 * @param argv The array of arguments. If NULL, returns NULL.
 * @return An Arguments_t containing the given arguments and a message error. If the message error is empty then nothing failed during parsing
 */
Arguments_t *parseArguments(int argc, char **argv);

/**
 * @param arguments The arguments struct
 * @return The file name of the arguments, or null if arguments is null.
 */
char *argumentsGetFileName(const Arguments_t *arguments);

/**
 * @param arguments The arguments struct
 * @return The cache size of the arguments, or 0 if arguments is null.
 */
u_int32_t argumentsGetCacheSize(const Arguments_t *arguments);

/**
 * @param arguments The arguments struct
 * @return The cache lines of the arguments, or 0 if arguments is null.
 */
u_int32_t argumentsGetLines(const Arguments_t *arguments);

/**
 * @param arguments The arguments struct
 * @return The cache sets of the arguments, or 0 if arguments is null.
 */
u_int32_t argumentsGetCacheSets(const Arguments_t *arguments);

/**
 * param arguments The arguments to be checked
 * @return True if the arguments have the verbose flag and both limits
 */
bool argumentsIsVerbose(const Arguments_t *arguments);

/**
 * @param arguments The arguments struct
 * @return The min operation to me processed as verbose, or 0 if arguments is null.
 */
u_int32_t argumentsGetMinVerboseOperation(const Arguments_t *arguments);

/**
 * @param arguments The arguments struct
 * @return The max operation to me processed as verbose, or 0 if arguments is null.
 */
u_int32_t argumentsGetMaxVerboseOperation(const Arguments_t *arguments);

/**
 * @param arguments The arguments struct
 * @return The error message, or null if arguments is null. This can be use whether to check if an error occurred during parsing or not. If the message is empty ("") then the parsing went successful.
 */
const char *argumentsGetErrorMessage(const Arguments_t *arguments);

/**
 * @param arguments The arguments struct
 * @param isSummary True if the function should return the summary output file
 * @return The name of the output file
 */
char *argumentsGetOutputFile(const Arguments_t *arguments);

/**
 * Frees the memory allocated by parseArguments
 * @param arguments The arguments to be freed
 */
void argumentsFree(Arguments_t *arguments);

#endif //ARGUMENTPARSER_H
