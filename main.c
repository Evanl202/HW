#include <stdio.h>
#include <string.h>
#include "process.h"
#include "utils.h"

int main() {
  Process processes[] = {
    {1,0,5},
    {2,1,3},
    {3,2,8},
    {4,3,6},
  };
  int n = sizeof(processes) / sizeof(processes[0]);
  Process processes_copy[n];
  memcpy(processes_copy, processes, sizeof(processes));

  int quantum = 4;
  printf("\nRound Robin Scheduler\n");
  roundRobin(processes, n, quantum);

  printf("\nMultilevel Feedback Queue\n");
  mlfq(processes_copy,n);

  return 0;
}
