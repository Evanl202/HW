#include <stdio.h>
#include "process.h"

void printGantt (int gantt[], int time_line[], int count) {
  printf("\nGantt Chart:\n");
  //loops through pid and prints 
  for(int i = 0; i < count; i++) {
    printf("| P%d ", gantt[i]);
  }
  printf("|\n");

  for(int i = 0; i <= count; i++) { //loops through pid completion time and prints
    printf("%d  ", time_line[i]);
  }
  printf("\n");
}

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
