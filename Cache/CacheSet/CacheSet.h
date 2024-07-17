#ifndef CACHESET_H
#define CACHESET_H

#include "../../CacheOperationInfo/CacheOperationInfo.h"
#include <sys/types.h>
#include <stdbool.h>

typedef struct CacheSet CacheSet_t;

/**
 * Allocates memory for a cache set and returns it
 * @param waysAmount The amount of lines the set has
 * @return A pointer to an allocated CacheSet
 */
CacheSet_t *cacheSetNew(u_int8_t waysAmount);

/**
 * Executes the given cache operation in the given cache set and modify the operationInfo to log everything that happened
 * @param cacheSet The cache set where the operation is going to be executed
 * @param tag The tag of the memory block in the set
 * @param accessNumber The number of the operation that accesed the address
 * @param operation The operation to be executed
 * @param operationInfo The operation info where the the function will log what happened
 */
CacheOperationInfo_t *cacheSetOperation(CacheSet_t *cacheSet,
                                        u_int32_t tag,
                                        u_int32_t accessNumber,
                                        CacheOperation_e operation);

/**
 * Frees the memory pointed by the given pointer
 * @param cacheSet The cache set to be freed
 */
void cacheSetFree(CacheSet_t *cacheSet);

#endif //CACHESET_H
