#include "Cache.h"
#include "CacheSet/CacheSet.h"
#include "../MemoryFails/HandleMemoryFail.h"
#include <malloc.h>
#include <math.h>

static const u_int32_t ALL_ONES_MASK = 0xffffffff;
static const u_int8_t ADDRESS_SIZE = 32;

struct Cache {
    CacheSet_t **sets;
    u_int32_t setsAmount;
    u_int32_t tagSize;
};

u_int32_t tagSize(u_int32_t cacheSets, u_int32_t cacheSize, u_int32_t cacheWays) {
    return ADDRESS_SIZE - (u_int32_t) log2(cacheSets) - (u_int32_t) log2(cacheSize / (cacheWays * cacheSets));
}

Cache_t *cacheNew(const u_int16_t cacheSize, const u_int16_t cacheSets, const u_int8_t cacheWays) {
    Cache_t *cache = (Cache_t *) malloc(sizeof(Cache_t));
    if (cache == NULL) handleMemoryFail();

    cache->sets = (CacheSet_t **) malloc(sizeof(CacheSet_t *) * cacheSets);
    if (cache->sets == NULL) handleMemoryFail();

    for (int i = 0; i < cacheSets; i++) cache->sets[i] = cacheSetNew(cacheWays);

    cache->setsAmount = cacheSets;

    cache->tagSize = tagSize(cacheSets, cacheSize, cacheWays);

    return cache;
}

CacheOperationInfo_t *cacheOperation(Cache_t *const restrict cache,
                                     const u_int32_t address,
                                     const u_int32_t accessNumber,
                                     const CacheOperation_e operation) {



    u_int32_t tagMask = ALL_ONES_MASK << (ADDRESS_SIZE - cache->tagSize);
    u_int32_t setMask = (~tagMask >> cacheGetBlockSize(cache)) << cacheGetBlockSize(cache);

    u_int32_t tag = (address & tagMask) >> (ADDRESS_SIZE - cache->tagSize);
    u_int32_t set = (address & setMask) >> cacheGetBlockSize(cache);

    CacheOperationInfo_t *operationInfo = cacheSetOperation(cache->sets[set], tag, accessNumber, operation);
    operationSetIndex(operationInfo, set);

    return operationInfo;
}

u_int32_t cacheGetBlockSize(const Cache_t *cache) {
    return ADDRESS_SIZE - cache->tagSize - (u_int32_t) log2(cache->setsAmount);
}

void cacheFree(Cache_t *cache) {
    for (u_int32_t i = 0; i < cache->setsAmount; i++) cacheSetFree(cache->sets[i]);
    free(cache->sets);
    free(cache);
}
