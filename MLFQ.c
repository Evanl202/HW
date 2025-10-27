#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "utils.h"

#define Q2 4
#define Q1 8

void mlfq(Process processes[], int n) {
  Node *q2_head = NULL, *q2_tail = NULL;
  Node *q1_head = NULL, *q1_tail = NULL;
  Node *q0_head = NULL, *q0_tail = NULL;

  int time = 0, completed = 0;
  int gantt[200], time_line[200];
  int count = 0;
//tracks when queue finishes and where to map PID to
  int finished_queue[n];
  int pid_index_map[1000];
  
  for (int i = 0; i < 1000; i++) pid_index_map[i] = -1;
  for (int i = 0; i < n; i++) {
    processes[i].remaining_time = processes[i].burst_time;
    processes[i].completed = 0;
    finished_queue[i] = 0;
  }
  for (int i = 0; i < n; i++) {
      if (processes[i].arrival_time == 0) 
          enqueue(&q2_head, &q2_tail, &processes[i]);
  }
  while (completed < n) {
      Process *p = NULL;
      int current_quantum = 0;
      Node **current_head = NULL, **current_tail = NULL;

      if (q2_head) {
          p = dequeue(&q2_head, &q2_tail);
          current_quantum = Q2;
          current_head = &q2_head;
          current_tail = &q2_tail;
      } else if (q1_head) {
          p = dequeue(&q1_head, &q1_tail);
          current_quantum = Q1;
          current_head = &q1_head;
          current_tail = &q1_tail;
      } else if (q0_head) {
          p = dequeue(&q0_head, &q0_tail);
          current_quantum = p->remaining_time;
          current_head = &q0_head;
          current_tail = &q0_tail;
      }
    //if no processes, increment time and check
      if (!p) {
        time++;
        for (int i = 0; i < n; i++) {
          if (processes[i].arrival_time <= time &&
              !processes[i].completed &&
              processes[i].remaining_time == processes[i].burst_time) {
              enqueue(&q2_head, &q2_tail, &processes[i]);
          }  
        }
        continue;
      }
    
      if (time < p->arrival_time)
        time = p->arrival_time;
      
      int exec_time = (p->remaining_time > current_quantum) ? current_quantum : p->remaining_time;
      
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
          enqueue(&q2_head, &q2_tail, &processes[i]);
        }
      }
      
      if (p->remaining_time == 0) {
        p-> completed = 1;
        p->turnaround_time = time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->burst_time;
        completed++;

        int idx = pid_index_map[p->pid];
        if (current_head == &q2_head && p->remaining_time == 0) finished_queue[idx] = 1;
        else if (current_head == &q1_head && p->remaining_time == 0) finished_queue[idx] = 2;
        else finished_queue[idx] = 3;
      } else {
          if(current_head == &q2_head)
            enqueue(&q1_head, &q1_tail, p);
          else if (current_head == &q1_head)
            enqueue(&q0_head, &q0_tail, p);
          else
            enqueue(&q0_head, &q0_tail, p);
      }
  }
  time_line[count] = time;

  printGantt(gantt, time_line, count);

    // Print finished queues
  printf("\nQueue 1: ");
  for (int i = 0; i < n; i++)
    if (finished_queue[i] == 1) printf("P%d | ", processes[i].pid);
  printf("\n");

  printf("Queue 2: ");
  for (int i = 0; i < n; i++)
    if (finished_queue[i] == 2) printf("P%d | ", processes[i].pid);
  printf("\n");

  printf("Queue 3: ");
  for (int i = 0; i < n; i++)
    if (finished_queue[i] == 3) printf("P%d | ", processes[i].pid);
  printf("\n");

  printResults(processes, n);
}
