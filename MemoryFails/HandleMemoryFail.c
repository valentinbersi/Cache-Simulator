#include "HandleMemoryFail.h"
#include <asm-generic/errno-base.h>
#include <stdlib.h>
#include <stdio.h>

#define MEMORY_FAIL "Memory Fail"

void handleMemoryFail(void) {
    fprintf(stderr, MEMORY_FAIL);
    exit(ENOMEM);
}
