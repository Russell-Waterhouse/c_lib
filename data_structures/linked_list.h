#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <stdlib.h>
#include <stdio.h>
#include "../types/types.h"

typedef u64 GENERIC_TYPE;

typedef struct Node {
  struct Node* next;
  GENERIC_TYPE value;
} Node;


Node* GENERIC_TYPE_insert_front(Node* head, GENERIC_TYPE value);

/* Returns the head node of a new list with the deleted item deleted. */
Node* GENERIC_TYPE_delete(Node* head, Node* delete_node);
#endif
