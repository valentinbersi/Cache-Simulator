#ifndef TRACEVALUES_H
#define TRACEVALUES_H

#include <bits/types/FILE.h>
#include "sys/types.h"
#include "../CacheOperationInfo//CacheOperationInfo.h"

typedef struct TraceValues TraceValues_t;

/**
 * @param traceValuesFile The file with the trace values
 * @return A TraceValues struct loaded with the next line of the given file.
 */
TraceValues_t *inputParserGetNextTraceValues(FILE *traceValuesFile);

/**
 * @param traceValues A TraceValues_t pointer that must to be initialized
 * @return The access operation to be concreted in the cache
 */
CacheOperation_e traceValuesGetType(const TraceValues_t *traceValues);

/**
 * Returns the address of the the TraceValues struct
 * @param traceValues The trace values where the address is going to be obtained
 * @return The address of the trace value
 */
u_int32_t traceValuesGetAddress(const TraceValues_t *traceValues);

#endif //TRACEVALUES_H
