#include <malloc.h>
#include "CacheOperationInfo.h"
#include "../MemoryFails/HandleMemoryFail.h"

struct CacheOperationInfo {
    CacheOperationInfo_e info;
    u_int32_t index;
    u_int32_t tag;
    u_int32_t line;
    int64_t previousTag;
    bool validBit;
    bool dirtyBit;
    u_int32_t lastUsed;
};

CacheOperationInfo_t *operationNew(void) {
    CacheOperationInfo_t *operationNew = (CacheOperationInfo_t *) malloc(sizeof(CacheOperationInfo_t));
    if (operationNew == NULL) handleMemoryFail();

    operationNew->info = NONE;
    operationNew->index = 0;
    operationNew->tag = 0;
    operationNew->line = 0;
    operationNew->previousTag = 0;
    operationNew->validBit = false;
    operationNew->dirtyBit = false;
    operationNew->lastUsed = 0;

    return operationNew;
}

CacheOperationInfo_e operationGetInfo(const CacheOperationInfo_t *const restrict operation) {
    if (operation == NULL) return NONE;
    return operation->info;
}

u_int32_t operationGetIndex(const CacheOperationInfo_t *const restrict operation) {
    if (operation == NULL) return 0;
    return operation->index;
}

u_int32_t operationGetTag(const CacheOperationInfo_t *const restrict operation) {
    if (operation == NULL) return 0;
    return operation->tag;
}

u_int32_t operationGetLine(const CacheOperationInfo_t *const restrict operation) {
    if (operation == NULL) return 0;
    return operation->line;
}

int64_t operationGetPreviousTag(const CacheOperationInfo_t *const restrict operation) {
    if (operation == NULL) return 0;
    return operation->previousTag;
}

bool operationGetValidBit(const CacheOperationInfo_t *const restrict operation) {
    if (operation == NULL) return false;
    return operation->validBit;
}

bool operationGetDirtyBit(const CacheOperationInfo_t *const restrict operation) {
    if (operation == NULL) return false;
    return operation->dirtyBit;
}

u_int32_t operationGetLastUsed(const CacheOperationInfo_t *const restrict operation) {
    if (operation == NULL) return 0;
    return operation->lastUsed;
}

void operationSetInfo(CacheOperationInfo_t *const restrict operation, CacheOperationInfo_e info) {
    if (operation != NULL) operation->info = info;
}

void operationSetIndex(CacheOperationInfo_t *const restrict operation, u_int32_t index) {
    if (operation != NULL) operation->index = index;
}

void operationSetTag(CacheOperationInfo_t *const restrict operation, u_int32_t tag) {
    if (operation != NULL) operation->tag = tag;
}

void operationSetLine(CacheOperationInfo_t *const restrict operation, u_int32_t line) {
    if (operation != NULL) operation->line = line;
}

void operationSetPreviousTag(CacheOperationInfo_t *const restrict operation, int64_t previousTag) {
    if (operation != NULL) operation->previousTag = previousTag;
}

void operationSetValidBit(CacheOperationInfo_t *const restrict operation, bool validBit) {
    if (operation != NULL) operation->validBit = validBit;
}

void operationSetDirtyBit(CacheOperationInfo_t *const restrict operation, bool dirtyBit) {
    if (operation != NULL) operation->dirtyBit = dirtyBit;
}

void operationSetLastUsed(CacheOperationInfo_t *const restrict operation, u_int32_t lastUsed) {
    if (operation != NULL) operation->lastUsed = lastUsed;
}
