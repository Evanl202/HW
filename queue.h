#ifndef QUEUE_H
#define QUEUE_H

#include "process.h"
//Class for Node and functions
typedef struct Node {
  Process *process;
  struct Node *next;
} Node;

//Queue functions
void enqueue(Node **head, Node **tail, Process *p);
Process* dequeue(Node **head, Node **tail);

#endif
