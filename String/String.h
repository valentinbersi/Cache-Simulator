#ifndef STRING_H
#define STRING_H

#include <sys/types.h>
#include <stdbool.h>

typedef enum NumericBase {
    DEC = 10,
    HEXA = 16
} NumericBase_e;

/**
 * Appends the src string to the end of the dst string. It's memory safe because it allocates more memory for the src to fit in the dst an frees the previous string.
 * @param dst A reference to the destination string
 * @param src The string to be concatenated
 */
void stringConcatString(char **dst, const char *src);

/**
 * Appends the number at the end of the dst string. It's memory safe, it allocates more memory for the number to fit in the dst an frees the previous string.
 * @param dst A reference to the destination string
 * @param number The number to be appended
 * @param base The base in which the number is going to be appended
 */
void stringConcatUnsigned(char **dst, u_int32_t number, NumericBase_e base);

/**
 * Returns the given string split by the delimiter as an array of strings.
 * @param string The string to be split
 * @param delimiter The delimiter
 * @return The string split by the delimiter
 */
char **split(char *string, char *delimiter);

void stringConcatFloat(char *string, float number);

#endif //STRING_H
