#include "CacheSet.h"
#include "../CacheLine/CacheLine.h"
#include "../../MemoryFails/HandleMemoryFail.h"
#include <malloc.h>

static const u_int8_t FIRST_LINE = 0;

struct CacheSet {
    CacheLine_t **lines;
    u_int8_t linesAmount;
};

CacheSet_t *cacheSetNew(const u_int8_t waysAmount) {
    CacheSet_t *cacheSet = (CacheSet_t *) malloc(sizeof(CacheSet_t));
    if (cacheSet == NULL) handleMemoryFail();

    cacheSet->lines = (CacheLine_t **) malloc(sizeof(CacheLine_t *) * waysAmount);
    if (cacheSet->lines == NULL) handleMemoryFail();

    for (int i = 0; i < waysAmount; i++) {
        cacheSet->lines[i] = cacheLineNew();
        if (cacheSet->lines[i] == NULL) handleMemoryFail();
    }

    cacheSet->linesAmount = waysAmount;

    return cacheSet;
}

/**
 * Search for the line that has to be allocated in case there´s a miss
 * @param cacheSet The cache set where the line will be searched
 * @return The line that hast to be allocated in case there's a miss
 */
u_int32_t searchAllocatable(const CacheSet_t *const restrict cacheSet) {
    u_int32_t minAccessNumber = cacheLineGetLastAccess(cacheSet->lines[FIRST_LINE]);
    u_int8_t index = 0;
    u_int8_t iterator = 0;
    bool found = false;

    while (!found && iterator < cacheSet->linesAmount) {
        if (!cacheLineIsValid(cacheSet->lines[iterator])) {
            found = true;
            index = iterator;
        } else if (cacheLineGetLastAccess(cacheSet->lines[iterator]) < minAccessNumber) {
            minAccessNumber = cacheLineGetLastAccess(cacheSet->lines[iterator]);
            index = iterator;
        }

        ++iterator;
    }

    return index;
}

/**
 * Finds the line with the given tag, if it doesn't find it, returns the line that has to be deallocated
 * @param cacheSet The cache set where the line will be searched
 * @param tag The tag to be searched
 * @return The line with the given tag, or if it doesn't find it, returns the line has to be deallocated
 */
u_int32_t cacheSetFindWay(const CacheSet_t *const restrict cacheSet, const u_int32_t tag) {
    bool found = false;
    int iterator = 0;
    while (!found && iterator < cacheSet->linesAmount) {
        found = cacheLineHasTag(cacheSet->lines[iterator], tag);
        ++iterator;
    }

    if (iterator >= cacheSet->linesAmount && !found) return searchAllocatable(cacheSet);
    return --iterator;
}

CacheOperationInfo_t *cacheSetOperation(CacheSet_t *const restrict cacheSet,
                                        const u_int32_t tag,
                                        const u_int32_t accessNumber,
                                        const CacheOperation_e operation) {

    u_int32_t wayIndex = cacheSetFindWay(cacheSet, tag);

    CacheOperationInfo_t *operationInfo = cacheLineOperation(cacheSet->lines[wayIndex],
                                                             tag,
                                                             accessNumber,
                                                             operation);
    operationSetLine(operationInfo, wayIndex);

    return operationInfo;
}

void cacheSetFree(CacheSet_t *const restrict cacheSet) {
    for (int i = 0; i < cacheSet->linesAmount; i++) cacheLineFree(cacheSet->lines[i]);
    free(cacheSet->lines);
    free(cacheSet);
}
