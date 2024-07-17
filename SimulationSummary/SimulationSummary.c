#include <stddef.h>
#include "SimulationSummary.h"
#include <malloc.h>
#include "../MemoryFails/HandleMemoryFail.h"

#define LOADS_STORES_FORMAT "loads %u stores %u total %u\n"
#define MISSES_FORMAT "rmiss %u wmiss %u total %u\n"
#define DIRTY_MISSES_FORMAT "dirty rmiss %u dirty wmiss %u\n"
#define BYTES_FORMAT "bytes read %u bytes written %u\n"
#define ACCESS_TIME_FORMAT "read time %u write time %u\n"
#define MISS_RATE_FORMAT "miss rate %f"

static const u_int8_t ACCESS_CACHE_TIME = 1;
static const u_int8_t AMOUNT_OF_ACCESS_ON_DIRTY = 2;
static const u_int8_t ACCESS_RAM_TIME = 100;


typedef struct SimulationSummary {
    u_int32_t loads;
    u_int32_t stores;
    u_int32_t readMisses;
    u_int32_t writeMisses;
    u_int32_t readDirtyMisses;
    u_int32_t writeDirtyMisses;
    u_int32_t readBytes;
    u_int32_t writtenBytes;
    u_int32_t readAccessTime;
    u_int32_t writeAccessTime;
} SimulationSummary_t;

SimulationSummary_t *simulationSummaryNew() {
    SimulationSummary_t *simulationSummary = malloc(sizeof(SimulationSummary_t));

    if (simulationSummary == NULL) handleMemoryFail();

    simulationSummary->loads = 0;
    simulationSummary->stores = 0;
    simulationSummary->readMisses = 0;
    simulationSummary->writeMisses = 0;
    simulationSummary->readDirtyMisses = 0;
    simulationSummary->writeDirtyMisses = 0;
    simulationSummary->readBytes = 0;
    simulationSummary->writtenBytes = 0;
    simulationSummary->readAccessTime = 0;
    simulationSummary->writeAccessTime = 0;

    return simulationSummary;
}

void simulationSummaryAddOperationValues(SimulationSummary_t *const restrict simulationSummary,
                                         const CacheOperationInfo_t *const restrict cacheOperationInfo,
                                         const TraceValues_t *const restrict traceValues,
                                         const u_int32_t blockSize) {

    if (simulationSummary == NULL || cacheOperationInfo == NULL || traceValues == NULL) return;

    switch (operationGetInfo(cacheOperationInfo)) {
        case MISS:
            if (traceValuesGetType(traceValues) == READ) {
                ++simulationSummary->readMisses;
                simulationSummary->readAccessTime += ACCESS_CACHE_TIME + ACCESS_RAM_TIME;
            } else {
                ++simulationSummary->writeMisses;
                simulationSummary->writeAccessTime += ACCESS_CACHE_TIME + ACCESS_RAM_TIME;
            }
            simulationSummary->readBytes += blockSize;
            break;

        case DIRTY_MISS:
            if (traceValuesGetType(traceValues) == READ) {
                ++simulationSummary->readDirtyMisses;
                ++simulationSummary->readMisses;
                simulationSummary->readAccessTime += ACCESS_CACHE_TIME + (AMOUNT_OF_ACCESS_ON_DIRTY * ACCESS_RAM_TIME);
            } else {
                ++simulationSummary->writeDirtyMisses;
                ++simulationSummary->writeMisses;
                simulationSummary->writeAccessTime += ACCESS_CACHE_TIME + (AMOUNT_OF_ACCESS_ON_DIRTY * ACCESS_RAM_TIME);

            }
            simulationSummary->writtenBytes += blockSize;
            simulationSummary->readBytes += blockSize;
            break;

        case HIT:
            traceValuesGetType(traceValues) == READ ? ++simulationSummary->readAccessTime
                                                    : ++simulationSummary->writeAccessTime;
            break;

        default:
            break;
    }

    if (traceValuesGetType(traceValues) == READ) ++simulationSummary->loads;
    else ++simulationSummary->stores;
}

/**
 * @param simulationSummary The simulation summary from where the total are going to be obtained
 * @return The sum of the loads and stores amount (same as simulationSummaryGetLoads() + simulationSummaryGetStores())
 */
u_int32_t simulationSummaryGetTotalOperations(const SimulationSummary_t *const restrict simulationSummary) {
    if (simulationSummary == NULL) return 0;
    return simulationSummary->stores + simulationSummary->loads;
}

/**
 * @param simulationSummary The simulation summary from where the total misses are going to be obtained
 * @return The sum of the read and write misses (same as simulationSummaryGetReadMisses() + simulationSummaryGetWriteMisses())
 */
u_int32_t simulationSummaryGetTotalMisses(const SimulationSummary_t *const restrict simulationSummary) {
    if (simulationSummary == NULL) return 0;
    return simulationSummary->writeMisses + simulationSummary->readMisses;
}

/**
 * @param simulationSummary The simulation summary from where the total dirty misses are going to be obtained
 * @return The sum of the read and write dirty misses (same as simulationSummaryGetReadDirtyMisses() + simulationSummaryGetWriteDirtyMisses())
 */
u_int32_t simulationSummaryGetTotalDirtyMisses(const SimulationSummary_t *simulationSummary) {
    return simulationSummary->readDirtyMisses + simulationSummary->writeDirtyMisses;
}

/**
 * @param simulationSummary The simulation summary from where the total dirty misses are going to be obtained
 * @return The rate of the misses
 */
float simulationSummaryGetMissRate(const SimulationSummary_t *simulationSummary) {
    if (simulationSummary == NULL) return 0;
    return (float) simulationSummaryGetTotalMisses(simulationSummary) /
           (float) simulationSummaryGetTotalOperations(simulationSummary);
}

void simulationSummaryPrint(const SimulationSummary_t *const restrict simulationSummary,
                            FILE *const restrict outputFile) {

    fprintf(outputFile,
            LOADS_STORES_FORMAT,
            simulationSummary->loads,
            simulationSummary->stores,
            simulationSummaryGetTotalOperations(simulationSummary));

    fprintf(outputFile,
            MISSES_FORMAT,
            simulationSummary->readMisses,
            simulationSummary->writeMisses,
            simulationSummaryGetTotalMisses(simulationSummary));

    fprintf(outputFile,
            DIRTY_MISSES_FORMAT,
            simulationSummary->readDirtyMisses,
            simulationSummary->writeDirtyMisses);

    fprintf(outputFile,
            BYTES_FORMAT,
            simulationSummary->readBytes,
            simulationSummary->writtenBytes);

    fprintf(outputFile,
            ACCESS_TIME_FORMAT,
            simulationSummary->readAccessTime,
            simulationSummary->writeAccessTime);

    fprintf(outputFile,
            MISS_RATE_FORMAT,
            simulationSummaryGetMissRate(simulationSummary));
}

void simulationSummaryFree(SimulationSummary_t *simulationSummary) {
    free(simulationSummary);
}
