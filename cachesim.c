#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ArgumentParser/ArgumentParser.h"
#include "Cache/Cache.h"
#include "SimulationSummary/SimulationSummary.h"

#define WRITE_MODE "w"
#define READ_MODE "r"
#define FILE_NOT_FOUND "The file could not be found"
#define STRING_FORMAT "%s"
#define INITIAL_VALUES_FORMAT "%u-way, %u sets, size = %dKB\n"

#define DIRECTLY_MAPPED_FORMAT "%i %s %x %x %i %i %b %b\n"
#define DIRECTLY_MAPPED_PREVIOUS_TAG_FORMAT "%i %s %x %x %i %x %b %b\n"
#define MORE_LINES_FORMAT "%i %s %x %x %i %i %b %b %u\n"
#define MORE_LINES_PREVIOUS_TAG_FORMAT "%i %s %x %x %i %x %b %b %u\n"

#define HIT_STRING "1"
#define MISS_STRING "2a"
#define DIRTY_MISS_STRING "2b"

#define NEWLINE "\n"
#define NO_ERRORS ""

static const u_int8_t EQUAL = 0;

static const u_int8_t ARG_ERROR = -1;
static const u_int8_t TRACE_ERROR = -1;

static const u_int8_t BASE = 2;
static const u_int8_t DIRECTLY_MAPPED_AMOUNT = 1;
static const u_int16_t BYTES_FOR_KB = 1024;

void printVerboseOperation(u_int32_t operationIndex,
                           CacheOperationInfo_t *operationInfo,
                           bool isDirectlyMapped,
                           FILE *const restrict outputFile) {

    if (outputFile == NULL) return;

    char *format = NULL;
    if (isDirectlyMapped) {
        if (operationGetPreviousTag(operationInfo) == -1) format = DIRECTLY_MAPPED_FORMAT;
        else format = DIRECTLY_MAPPED_PREVIOUS_TAG_FORMAT;
    } else {
        if (operationGetPreviousTag(operationInfo) == -1) format = MORE_LINES_FORMAT;
        else format = MORE_LINES_PREVIOUS_TAG_FORMAT;
    }

    char *operation =
            operationGetInfo(operationInfo) == HIT ? HIT_STRING
                                                   : (operationGetInfo(operationInfo) == MISS ? MISS_STRING
                                                                                              : DIRTY_MISS_STRING);

    fprintf(outputFile,
            format,
            operationIndex,
            operation,
            operationGetIndex(operationInfo),
            operationGetTag(operationInfo),
            operationGetLine(operationInfo),
            operationGetPreviousTag(operationInfo),
            operationGetValidBit(operationInfo),
            operationGetDirtyBit(operationInfo),
            operationGetLastUsed(operationInfo));
}

/**
 * Returns true if n is in between min value and max value (including both of them), works only with positive numbers
 * @param n The value to be checked
 * @param minValue The min value that n can take
 * @param maxValue The max value that n can take
 * @return True if n is in between min value and max value
 */
bool isInBetween(u_int32_t n, u_int32_t minValue, u_int32_t maxValue) {
    return n <= maxValue && n >= minValue;
}

SimulationSummary_t *simulation(Cache_t *const restrict cache,
                                FILE *const restrict traceFile,
                                u_int32_t minVerboseOperation,
                                u_int32_t maxVerboseOperation,
                                bool isVerbose,
                                bool isDirectlyMapped,
                                FILE *verboseOutput) {

    TraceValues_t *traceValues = inputParserGetNextTraceValues(traceFile);
    u_int32_t operationIndex = 0;
    CacheOperation_e operation;
    CacheOperationInfo_t *operationInfo;
    SimulationSummary_t *simulationSummary = simulationSummaryNew();

    while (traceValues != NULL) {
        operation = traceValuesGetType(traceValues);

        operationInfo = cacheOperation(cache, traceValuesGetAddress(traceValues), operationIndex, operation);

        if (isInBetween(operationIndex, minVerboseOperation, maxVerboseOperation) && isVerbose)
            printVerboseOperation(operationIndex, operationInfo, isDirectlyMapped, verboseOutput);

        simulationSummaryAddOperationValues(simulationSummary,
                                            operationInfo,
                                            traceValues,
                                            (u_int32_t) pow(BASE, cacheGetBlockSize(cache)));

        free(operationInfo);
        ++operationIndex;
        free(traceValues);
        traceValues = inputParserGetNextTraceValues(traceFile);
    }

    return simulationSummary;
}

void printSimulation(FILE *outputFile,
                     u_int32_t cacheLines,
                     u_int32_t cacheSize,
                     u_int32_t cacheSets,
                     const SimulationSummary_t *simulationSummary) {


    if (outputFile == NULL || simulationSummary == NULL) return;

    fprintf(outputFile, INITIAL_VALUES_FORMAT, cacheLines, cacheSets, cacheSize / BYTES_FOR_KB);
    simulationSummaryPrint(simulationSummary, outputFile);
    fprintf(outputFile, NEWLINE);
}

int main(int argc, char **argv) {
    Arguments_t *arguments = parseArguments(argc, argv);

    const char *errorMessage = argumentsGetErrorMessage(arguments);

    if (strcmp(errorMessage, NO_ERRORS) != EQUAL) {
        fprintf(stderr, STRING_FORMAT, errorMessage);
        argumentsFree(arguments);
        return ARG_ERROR;
    }

    Cache_t *cache = cacheNew(argumentsGetCacheSize(arguments),
                              argumentsGetCacheSets(arguments),
                              argumentsGetLines(arguments));


    FILE *traceFile = fopen(argumentsGetFileName(arguments), READ_MODE);

    if (traceFile == NULL) {
        fprintf(stderr, FILE_NOT_FOUND);
        argumentsFree(arguments);
        cacheFree(cache);
        return TRACE_ERROR;
    }

    char *outputFileName = argumentsGetOutputFile(arguments);
    FILE *outputFile = fopen(outputFileName, WRITE_MODE);
    free(outputFileName);

    SimulationSummary_t *simulationSummary = simulation(cache,
                                                        traceFile,
                                                        argumentsGetMinVerboseOperation(arguments),
                                                        argumentsGetMaxVerboseOperation(arguments),
                                                        argumentsIsVerbose(arguments),
                                                        argumentsGetLines(arguments) == DIRECTLY_MAPPED_AMOUNT,
                                                        outputFile);

    printSimulation(outputFile,
                    argumentsGetLines(arguments),
                    argumentsGetCacheSize(arguments),
                    argumentsGetCacheSets(arguments),
                    simulationSummary);

    simulationSummaryFree(simulationSummary);
    argumentsFree(arguments);
    cacheFree(cache);
    fclose(outputFile);
    fclose(traceFile);

    return 0;
}
