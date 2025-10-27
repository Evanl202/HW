#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "utils.h"

#define Q2 4   // Time quantum for Queue 1 (highest priority)
#define Q1 8   // Time quantum for Queue 2 (medium priority)

void mlfq(Process processes[], int n) {
    Node *q2_head = NULL, *q2_tail = NULL;
    Node *q1_head = NULL, *q1_tail = NULL;
    Node *q0_head = NULL, *q0_tail = NULL;

    int time = 0, completed = 0;
    int gantt[200], time_line[200];
    int count = 0;

    int finished_queue[n];
    int pid_index_map[1000];

    // Initialize PID mapping
    for (int i = 0; i < 1000; i++) pid_index_map[i] = -1;
    for (int i = 0; i < n; i++) pid_index_map[processes[i].pid] = i;

    // Initialize processes
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].completed = 0;
        finished_queue[i] = 0;
    }

    // Enqueue processes that arrive at time 0
    for (int i = 0; i < n; i++)
        if (processes[i].arrival_time == 0)
            enqueue(&q2_head, &q2_tail, &processes[i]);

    while (completed < n) {
        Process *p = NULL;
        int current_quantum = 0;
        int queue_level = 0;  // 1 = Q2, 2 = Q1, 3 = Q0

        if (q2_head) {
            p = dequeue(&q2_head, &q2_tail);
            current_quantum = Q2;
            queue_level = 1;
        } else if (q1_head) {
            p = dequeue(&q1_head, &q1_tail);
            current_quantum = Q1;
            queue_level = 2;
        } else if (q0_head) {
            p = dequeue(&q0_head, &q0_tail);
            current_quantum = p->remaining_time;
            queue_level = 3;
        }

        // No ready process, advance time
        if (!p) {
            time++;
            for (int i = 0; i < n; i++)
                if (processes[i].arrival_time <= time &&
                    !processes[i].completed &&
                    processes[i].remaining_time == processes[i].burst_time)
                    enqueue(&q2_head, &q2_tail, &processes[i]);
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

        // Enqueue newly arrived processes to Q2
        for (int i = 0; i < n; i++)
            if (processes[i].arrival_time <= time &&
                !processes[i].completed &&
                processes[i].remaining_time == processes[i].burst_time &&
                &processes[i] != p)
                enqueue(&q2_head, &q2_tail, &processes[i]);

        // Process finished
        if (p->remaining_time == 0) {
            p->completed = 1;
            p->turnaround_time = time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->burst_time;
            completed++;

            int idx = pid_index_map[p->pid];
            finished_queue[idx] = queue_level;
        } else {
            // Demote unfinished processes
            if (queue_level == 1) enqueue(&q1_head, &q1_tail, p);
            else if (queue_level == 2) enqueue(&q0_head, &q0_tail, p);
            else enqueue(&q0_head, &q0_tail, p);
        }
    }

    time_line[count] = time;

    // Print Gantt chart
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
