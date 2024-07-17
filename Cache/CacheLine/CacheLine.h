#ifndef CACHEWAY_H
#define CACHEWAY_H

#include <sys/types.h>
#include <stdbool.h>
#include "../../CacheOperationInfo/CacheOperationInfo.h"

typedef struct CacheLine CacheLine_t;

/**
 * Allocates memory for a new cacheLine and returns a pointer to it
 * @return A pointer to an allocated cacheLine
 */
CacheLine_t *cacheLineNew(void);

/**
 * Executes the given operation on the given cache line
 * @param cacheLine The cache line where the operation is going to be executed
 * @param tag The tag to be operated with
 * @param access The index of the operation that is accessing this line
 * @param operation The operation to be executed
 * @param operationInfo The operation info where the line will log what happened during the execution
 */
CacheOperationInfo_t *cacheLineOperation(CacheLine_t *cacheLine,
                                         u_int32_t tag,
                                         u_int32_t access,
                                         CacheOperation_e operation);

/**
 * @param cacheLine The cache line where the last access is going to be obtained
 * @return The index of the instruction that last accessed this line
 */
u_int32_t cacheLineGetLastAccess(const CacheLine_t *cacheLine);

/**
 * @param cacheLine The cache line where the tag will be asked
 * @param tag The tag to be asked
 * @return True if the given line has the given tag
 */
bool cacheLineHasTag(const CacheLine_t *cacheLine, u_int32_t tag);

/**
 * @param cacheLine The line to be checked
 * @return True if the line is valid
 */
bool cacheLineIsValid(const CacheLine_t *cacheLine);

/**
 * Frees the memory pointed by the given pointer
 * @param cacheLine The chatelaine to be freed
 */
void cacheLineFree(CacheLine_t *cacheLine);

#endif //CACHEWAY_H
