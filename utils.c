#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "utils.h"

//prints result 
void printResults (Process processes[], int n) {
  double total_wt = 0, total_tat = 0;
  
  printf("\n PID\tAT\tBT\tWT\tTAT\n");
  
  for (int i = 0; i < n; i++) {
    printf("%d\t%d\t%d\t%d\t%d\n",
        processes[i].pid,
        processes[i].arrival_time,
        processes[i].burst_time,
        processes[i].waiting_time,
        processes[i].turnaround_time);
      
      total_wt += processes[i].waiting_time;
      total_tat += processes[i].turnaround_time;
  }
  printf("\nAverage waiting time = %.2f\n", total_wt / n);
  printf("Average turnaround time = %.2f\n", total_tat / n);
}
