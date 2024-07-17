#ifndef SIMULATIONSUMMARY_H
#define SIMULATIONSUMMARY_H

#include <bits/types/FILE.h>
#include "../CacheOperationInfo//CacheOperationInfo.h"
#include "../InputParser/TraceValues.h"

typedef struct SimulationSummary SimulationSummary_t;

/**
 * Allocates memory for a new simulation Summary and returns a pointer to it
 * @return A pointer to an allocated simulation summary
 */
SimulationSummary_t *simulationSummaryNew(void);

/**
 * Adds the operation values of the given operation with the given info to the simulationSummary
 * @param simulationSummary The simulation summary to be updated
 * @param cacheOperationInfo The Operation Info
 * @param traceValues The trace values of the operation
 */
void simulationSummaryAddOperationValues(SimulationSummary_t *simulationSummary,
                                         const CacheOperationInfo_t *cacheOperationInfo,
                                         const TraceValues_t *traceValues,
                                         u_int32_t blockSize);

/**
 * Prints the summary on the output file
 * @param simulationSummary The simulation summary to be printed
 * @param outputFile The output file where the simulation is going to be printed
 */
void simulationSummaryPrint(const SimulationSummary_t *simulationSummary, FILE *outputFile);

/**
 * Frees the memory pointed by the given pointer
 * @param simulationSummary The simulation summary to be freed
 */
void simulationSummaryFree(SimulationSummary_t *simulationSummary);

#endif //SIMULATIONSUMMARY_H
