#ifndef UTILS_H
#define UTILS_H

#include "process.h"

// Prints a Gantt chart
void printGantt(int gantt[], int time_line[], int count);

// Prints waiting time, turnaround time, and averages
void printResults(Process processes[], int n);

#endif
