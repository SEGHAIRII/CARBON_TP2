#ifndef LOWER_BOUNDS_H
#define LOWER_BOUNDS_H

#include "Problem.h"
#include <vector>

// Compute the completion times for the current partial schedule on each machine.
std::vector<int> computeScheduleCompletion(const Problem &prob, const std::vector<int> &schedule);

// For a given machine index, compute the minimum extra processing (tail) over unscheduled jobs.
// That is, for machine index 'machine' (0-indexed), compute:
//   tail(machine) = min_{j unscheduled} { sum_{l = machine+1}^{m-1} p_{j,l} }
// If no unscheduled jobs remain, return 0.
int computeTailMin(const Problem &prob, const std::vector<bool> &scheduled, int machine);

// Compute the total processing time on a given machine for all unscheduled jobs.
int computeSumForMachine(const Problem &prob, const std::vector<bool> &scheduled, int machine);

// LB1: one-machine lower bound using completion times, unscheduled work and tail.
int computeLB1(const Problem &prob, const std::vector<int> &schedule, const std::vector<bool> &scheduled);

// LB2: two-machine lower bound that refines LB1 by considering each pair of machines.
int computeLB2(const Problem &prob, const std::vector<int> &schedule, const std::vector<bool> &scheduled);

// Compute the optimal makespan of a two-machine flow-shop problem (using Johnson's rule)
// for the unscheduled jobs, where machine indices are machineK and machineL.
int computeJohnsonMakespan(const Problem &prob, const std::vector<bool> &scheduled, int machineK, int machineL);

#endif // LOWER_BOUNDS_H
