#include "CacheLine.h"
#include "../../MemoryFails/HandleMemoryFail.h"
#include <malloc.h>
#include <string.h>

static const int64_t INITIAL_TAG_VALUE = -1;
static const int64_t INITIAL_ACCESS_VALUE = 0;

struct CacheLine {
    bool valid;
    bool dirty;
    u_int32_t access;
    int64_t tag;
};

CacheLine_t *cacheLineNew(void) {
    CacheLine_t *way = (CacheLine_t *) malloc(sizeof(CacheLine_t));
    if (way == NULL) handleMemoryFail();

    way->valid = false;
    way->dirty = false;
    way->tag = INITIAL_TAG_VALUE;
    way->access = INITIAL_ACCESS_VALUE;

    return way;
}

/**
 * Handles the case where the cacheLine is not valid
 * @param cacheLine The cache line that is not valid
 * @param operationInfo The operation info where the the function will log the case
 */
void cacheLineHandleNotValid(CacheLine_t *const restrict cacheLine,
                             CacheOperationInfo_t *const restrict operationInfo) {

    operationSetInfo(operationInfo, MISS);
    cacheLine->valid = true;
}

/**
 * Handles the case where the line is read
 * @param cacheLine The cacheline to be read
 * @param tag The tag to be read
 * @param operationInfo The operation info where the function will log what happens during reading
 */
void cacheLineHandleRead(CacheLine_t *const restrict cacheLine,
                         u_int32_t tag,
                         CacheOperationInfo_t *const restrict operationInfo) {

    if (cacheLine->tag != tag) {
        if (cacheLine->dirty) operationSetInfo(operationInfo, DIRTY_MISS);
        else operationSetInfo(operationInfo, MISS);
        cacheLine->dirty = false;
        return;
    }

    operationSetInfo(operationInfo, HIT);
}

/**
 * Handles the case where the line is written
 * @param cacheLine The cache line to be written
 * @param tag The tag to be written
 * @param operationInfo The operation info where the function will log what happens during writing
 */
void cacheLineHandleWrite(CacheLine_t *const restrict cacheLine,
                          u_int32_t tag,
                          CacheOperationInfo_t *const restrict operationInfo) {

    if (cacheLine->tag != tag) {
        if (cacheLine->dirty) operationSetInfo(operationInfo, DIRTY_MISS);
        else operationSetInfo(operationInfo, MISS);
        cacheLine->dirty = true;
        return;
    }

    operationSetInfo(operationInfo, HIT);
    cacheLine->dirty = true;
}

CacheOperationInfo_t *cacheLineOperation(CacheLine_t *const restrict cacheLine,
                                         u_int32_t tag,
                                         u_int32_t access,
                                         CacheOperation_e operation) {

    if (cacheLine == NULL) return NULL;

    CacheOperationInfo_t *operationInfo = operationNew();

    operationSetTag(operationInfo, tag);
    operationSetPreviousTag(operationInfo, cacheLine->tag);
    operationSetValidBit(operationInfo, cacheLine->valid);
    operationSetDirtyBit(operationInfo, cacheLine->dirty);
    operationSetLastUsed(operationInfo, cacheLine->access);

    if (!cacheLineIsValid(cacheLine)) cacheLineHandleNotValid(cacheLine, operationInfo);

    if (operation == READ) cacheLineHandleRead(cacheLine, tag, operationInfo);
    else cacheLineHandleWrite(cacheLine, tag, operationInfo);

    cacheLine->tag = tag;
    cacheLine->access = access;
    return operationInfo;
}

u_int32_t cacheLineGetLastAccess(const CacheLine_t *const restrict cacheLine) {
    return cacheLine->access;
}

bool cacheLineHasTag(const CacheLine_t *const restrict cacheLine, u_int32_t tag) {
    return cacheLine->tag == tag;
}

bool cacheLineIsValid(const CacheLine_t *const restrict cacheLine) {
    return cacheLine->valid;
}

void cacheLineFree(CacheLine_t *const restrict cacheLine) {
    free(cacheLine);
}
