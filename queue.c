#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "queue.h"

//Linked list implementation of enqueue from tail
void enqueue(Node **head, Node **tail, Process *p) {
  Node *newNode = (Node*)malloc(sizeof(Node));
  newNode->process = p;
  newNode->next = NULL;

  if (*tail) {
    (*tail)->next = newNode; 
  } else{
      *head = newNode;
  }
  *tail = newNode;
}
//Dequeue function from head
Process* dequeue(Node **head, Node **tail) {
  if (!*head) return NULL;

  Node *temp = *head;
  Process *p = temp->process;
  *head = (*head)->next;
  if (!*head) *tail = NULL;

  free(temp);
  return p;
}
