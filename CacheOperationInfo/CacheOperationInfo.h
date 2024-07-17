#ifndef OPERATIONSTATUS_H
#define OPERATIONSTATUS_H

#include <sys/types.h>
#include <stdbool.h>

typedef enum CacheOperation {
    READ = 'R',
    WRITE = 'W'
} CacheOperation_e;

typedef enum {
    HIT = 0,
    MISS = 1,
    DIRTY_MISS = 2,
    NONE = 3
} CacheOperationInfo_e;

typedef struct CacheOperationInfo CacheOperationInfo_t;

/**
 * @return A memory allocated empty cache operation
 */
CacheOperationInfo_t *operationNew(void);

/**
 * @param operation The operation where the info is going to be obtained
 * @return The operation info (if it was a HIT, a MISS or a DIRTY_MISS)
 */
CacheOperationInfo_e operationGetInfo(const CacheOperationInfo_t *operation);

/**
 * @param operation The operation from where the set index is going to be obtained
 * @return The set index of the CacheOperationInfo
 */
u_int32_t operationGetIndex(const CacheOperationInfo_t *operation);

/**
 * @param operation The operation from where the mem tag is going to be obtained
 * @return The mem tag of the CacheOperationInfo
 */
u_int32_t operationGetTag(const CacheOperationInfo_t *operation);

/**
 * @param operation The operation from where line index is going to be obtained
 * @return The line index of the CacheOperationInfo
 */
u_int32_t operationGetLine(const CacheOperationInfo_t *operation);

/**
 * @param operation The operation from where the previous tag is going to be obtained
 * @return The previos tag of the CacheOperationInfo
 */
int64_t operationGetPreviousTag(const CacheOperationInfo_t *operation);

/**
 * @param operation The operation from where the valid bit is going to be obtained
 * @return The valid bit of the CacheOperationInfo
 */
bool operationGetValidBit(const CacheOperationInfo_t *operation);

/**
 * @param operation The operation from where the dirty bit is going to be obtained
 * @return The dirty bit of the CacheOperationInfo
 */
bool operationGetDirtyBit(const CacheOperationInfo_t *operation);

/**
 * @param operation The operation from where the last used index is going to be obtained
 * @return The last used index of the CacheOperationInfo
 */
u_int32_t operationGetLastUsed(const CacheOperationInfo_t *operation);

/**
 * Sets the info of the given Cache Operation
 * @param operation The CacheOperationInfo whose info is going to be set
 * @param info The info
 */
void operationSetInfo(CacheOperationInfo_t *operation, CacheOperationInfo_e info);

/**
 * Sets the index of the given Cache Operation
 * @param operation The CacheOperationInfo whose index is going to be set
 * @param index The index
 */
void operationSetIndex(CacheOperationInfo_t *operation, u_int32_t index);

/**
 * Sets the tag of the given Cache Operation
 * @param operation The CacheOperationInfo whose tag is going to be set
 * @param tag The tag
 */
void operationSetTag(CacheOperationInfo_t *operation, u_int32_t tag);

/**
 * Sets the line of the given Cache Operation
 * @param operation The CacheOperationInfo whose line is going to be set
 * @param line The line
 */
void operationSetLine(CacheOperationInfo_t *operation, u_int32_t line);

/**
 * Sets the previos tag of the given Cache Operation
 * @param operation The CacheOperationInfo whose previous tag is going to be set
 * @param previousTag The previous tag
 */
void operationSetPreviousTag(CacheOperationInfo_t *operation, int64_t previousTag);

/**
 * Sets the valid bit of the given Cache Operation
 * @param operation The CacheOperationInfo whose valid bit is going to be set
 * @param validBit The valid bit
 */
void operationSetValidBit(CacheOperationInfo_t *operation, bool validBit);

/**
 * Sets the dirty bit of the given Cache Operation
 * @param operation The CacheOperationInfo whose dirty bit is going to be set
 * @param dirtyBit The dirty bit
 */
void operationSetDirtyBit(CacheOperationInfo_t *operation, bool dirtyBit);

/**
 * Sets the last used of the given Cache Operation
 * @param operation The CacheOperationInfo whose valid bit is going to be set
 * @param lastUsed The last used operation index
 */
void operationSetLastUsed(CacheOperationInfo_t *operation, u_int32_t lastUsed);

/**
 * Frees the memory pointed by the given operation
 * @param operation The operation to be freed
 */
void operationFree(CacheOperationInfo_t *operation);

#endif //OPERATIONSTATUS_H
