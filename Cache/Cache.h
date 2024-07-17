#ifndef CACHELAB_CACHE_H
#define CACHELAB_CACHE_H

#include <sys/types.h>
#include <stdbool.h>
#include "../CacheOperationInfo//CacheOperationInfo.h"


typedef struct Cache Cache_t;

/**
 * Allocates memory for a cache and returns a pointer to that memory
 * @param cacheSize The size of the cache
 * @param cacheSets The sets of the cache
 * @param cacheWays The lines of the cache
 * @return A pointer to a new cache
 */
Cache_t *cacheNew(u_int16_t cacheSize, u_int16_t cacheSets, u_int8_t cacheWays);

/**
 * Executes the given cache operation in the given cache
 * @param cache The cache where the operation is going to be executed
 * @param address The address where the operation is going to be executed
 * @param accessNumber The index of the operation
 * @param operation The operation to do
 * @return A CacheOperationInfo that contains a status log of what happened during the execution
 */
CacheOperationInfo_t *cacheOperation(Cache_t *cache,
                                     u_int32_t address,
                                     u_int32_t accessNumber,
                                     CacheOperation_e operation);

/**
 * Returns the block size of the cache
 * @param cache The cache from where the block size is going to be obtained
 * @return The block size of the cache
 */
u_int32_t cacheGetBlockSize(const Cache_t *cache);

/**
 * Frees the memory pointed by the given pointer
 * @param cache A pointer to the Cache that will be freed
 */
void cacheFree(Cache_t *cache);

#endif //CACHELAB_CACHE_H
