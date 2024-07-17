#ifndef FILEPROCESS_H
#define FILEPROCESS_H

#include <bits/types/FILE.h>

/**
 * Stores in dst a full line of the given file and returns 0 if the file has ended.
 * @param dst The dst string, where the line is going to be stores. If null, the function returns 0.
 * @param file The file from where the line is going to be obtained. If null, the function returns 0.
 * @return 0 if the file has ended.
 */
int getLine(char **dst, FILE *file);

#endif //FILEPROCESS_H
