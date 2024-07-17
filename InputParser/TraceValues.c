#include "TraceValues.h"
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "../MemoryFails/HandleMemoryFail.h"
#include "../FileProcess/FileProcess.h"
#include "../String/String.h"

#define SPACE " "
#define TYPE 1
#define ADDRESS 2
#define BLANK ""
#define READ_TYPE "R"
#define EQUAL 0

struct TraceValues {
    CacheOperation_e type;
    u_int32_t ref_address;
};


TraceValues_t *traceValuesNew(CacheOperation_e type, u_int32_t address) {
    TraceValues_t *traceValues = (TraceValues_t *) malloc(sizeof(TraceValues_t));
    if (traceValues == NULL) handleMemoryFail();

    traceValues->type = type;
    traceValues->ref_address = address;

    return traceValues;
}

TraceValues_t *inputParserGetNextTraceValues(FILE *file) {
    char *line = NULL;
    TraceValues_t *traceValue = NULL;
    if (getLine(&line, file)) {
        char **splitLine = split(line, SPACE);
        char *errorHandler = NULL;
        CacheOperation_e type = strcmp(splitLine[TYPE], READ_TYPE) == EQUAL ? READ : WRITE;
        u_int32_t address = strtol(splitLine[ADDRESS], &errorHandler, HEXA);
        traceValue = traceValuesNew(type, address);

        free(splitLine);
    }

    free(line);

    return traceValue;
}

CacheOperation_e traceValuesGetType(const TraceValues_t *traceValues) {
    if (traceValues == NULL) return 0;
    return traceValues->type;
}

u_int32_t traceValuesGetAddress(const TraceValues_t *traceValues) {
    if (traceValues == NULL) return 0;
    return traceValues->ref_address;
}
