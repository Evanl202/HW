#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "utils.h"

#define Q1 4  // Highest priority
#define Q2 8  // Medium priority

void mlfq(Process processes[], int n) {
    Node *q1_head = NULL, *q1_tail = NULL;
    Node *q2_head = NULL, *q2_tail = NULL;
    Node *q3_head = NULL, *q3_tail = NULL;

    int gantt[200], time_line[200], count = 0;
    int time = 0, completed = 0;
    int finished_queue[n];
    int pid_index_map[1000];

    // Initialize PID to index map and finished queues
    for (int i = 0; i < 1000; i++) pid_index_map[i] = -1;
    for (int i = 0; i < n; i++) {
        pid_index_map[processes[i].pid] = i;
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].completed = 0;
        finished_queue[i] = 0;
    }

    // Enqueue initial processes into Q1
    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time == 0)
            enqueue(&q1_head, &q1_tail, &processes[i]);
    }

    while (completed < n) {
        Process *p = NULL;
        int current_quantum = 0;
        int current_queue = 0; // 1 = Q1, 2 = Q2, 3 = Q3

        // Pick process from highest priority non-empty queue
        if (q1_head) { p = dequeue(&q1_head, &q1_tail); current_quantum = Q1; current_queue = 1; }
        else if (q2_head) { p = dequeue(&q2_head, &q2_tail); current_quantum = Q2; current_queue = 2; }
        else if (q3_head) { p = dequeue(&q3_head, &q3_tail); current_quantum = p->remaining_time; current_queue = 3; }

        if (!p) {
            // No process ready, advance time
            time++;
            for (int i = 0; i < n; i++) {
                if (!processes[i].completed && processes[i].arrival_time <= time && processes[i].remaining_time == processes[i].burst_time)
                    enqueue(&q1_head, &q1_tail, &processes[i]);
            }
            continue;
        }

        if (time < p->arrival_time) time = p->arrival_time;

        int exec_time = (p->remaining_time > current_quantum) ? current_quantum : p->remaining_time;

        gantt[count] = p->pid;
        time_line[count] = time;
        count++;

        time += exec_time;
        p->remaining_time -= exec_time;

        // Add newly arrived processes to Q1
        for (int i = 0; i < n; i++) {
            if (!processes[i].completed && processes[i].arrival_time <= time && processes[i].remaining_time == processes[i].burst_time && &processes[i] != p)
                enqueue(&q1_head, &q1_tail, &processes[i]);
        }

        // Process finished
        if (p->remaining_time == 0) {
            p->completed = 1;
            p->turnaround_time = time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->burst_time;
            completed++;

            int idx = pid_index_map[p->pid];
            finished_queue[idx] = current_queue;
        } else {
            // Demote unfinished processes
            if (current_queue == 1) enqueue(&q2_head, &q2_tail, p);
            else if (current_queue == 2) enqueue(&q3_head, &q3_tail, p);
            else enqueue(&q3_head, &q3_tail, p);
        }
    }

    time_line[count] = time;
    printGantt(gantt, time_line, count);

    // Print queues where processes finished
    printf("\nQueue 1: ");
    for (int i = 0; i < n; i++) if (finished_queue[i] == 1) printf("P%d | ", processes[i].pid);
    printf("\nQueue 2: ");
    for (int i = 0; i < n; i++) if (finished_queue[i] == 2) printf("P%d | ", processes[i].pid);
    printf("\nQueue 3: ");
    for (int i = 0; i < n; i++) if (finished_queue[i] == 3) printf("P%d | ", processes[i].pid);
    printf("\n");

    printResults(processes, n);
}
