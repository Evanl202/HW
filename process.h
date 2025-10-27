#ifndef PROCESS_H
#define PROCESS_H

//Structure of process
typedef struct {
  int pid;
  int arrival_time;
  int burst_time;
  int waiting_time;
  int turnaround_time;
  int completed;
  int remaining_time;
} Process;

#endif
