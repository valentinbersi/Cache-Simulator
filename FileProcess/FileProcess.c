#include "FileProcess.h"
#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>

static const u_int8_t NULL_CHAR_SIZE = 1;
static const char NEW_LINE = '\n';

int getLine(char **dst, FILE *file) {
    if (dst == NULL || file == NULL) return 0;

    free(*dst);

    fpos_t position;
    fgetpos(file, &position);

    char currentCharacter = (char) fgetc(file);
    size_t lineSize = 0;

    while (currentCharacter != NEW_LINE && !feof(file)) {
        lineSize++;
        currentCharacter = (char) fgetc(file);
    }

    if (!feof(file)) *dst = malloc(lineSize + NULL_CHAR_SIZE);

    fsetpos(file, &position);

    if (!feof(file)) getline(dst, &lineSize, file);

    return !feof(file);
}
