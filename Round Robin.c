#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "utils.h"

void roundRobin(Process processes[], int n, int quantum) {
  Node *head = NULL, *tail = NULL;
  int time = 0, completed = 0;
  int gantt[100], time_line[100];
  int count = 0;
  // for (int i = 0; i < n; i++) {
  //   processes[i].remaining_time = processes[i].burst_time;
  //   processes[i].completed = 0;
  // }
  for (int i = 0; i < n; i++) {
      if (processes[i].arrival_time == 0) {
          enqueue(&head, &tail, &processes[i]);
      }
  }
    //Subtract time using quantum time
  while (completed < n) {
    if (head) { 
      Process *p = dequeue(&head, &tail);

      if (time < p->arrival_time)
        time = p->arrival_time;
      
      int exec_time = (p->remaining_time > quantum) ? quantum : p->remaining_time;
      
      gantt[count] = p->pid;
      time_line[count] = time;
      count++;

      time += exec_time;
      p->remaining_time -= exec_time;

      for (int i = 0; i < n; i++) {
        if(processes[i].arrival_time <= time &&
          !processes[i].completed &&
          processes[i].remaining_time == processes[i].burst_time &&
          &processes[i] != p) {
          enqueue(&head, &tail, &processes[i]);
        }
      }
      
      if (p->remaining_time > 0) {
        enqueue(&head, &tail, p);
      } else {
        p-> completed = 1;
        p->turnaround_time = time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->burst_time;

        completed++;
      }
    } else {
        time++;
        for (int i = 0; i < n; i++) {
          if(processes[i].arrival_time <= time &&
            !processes[i].completed &&
            processes[i].remaining_time == processes[i].burst_time) {
            enqueue(&head, &tail, &processes[i]);
          }
        }
    }
  }

  time_line[count] = time;

  printGantt(gantt, time_line, count);
  printResults(processes, n);
}
