#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

Node* GENERIC_TYPE_insert_front(Node* head, GENERIC_TYPE value) {
  Node* node;
  node = malloc(sizeof(*node));
  if (NULL == node) {
    printf("Failed to allocate memory for node");
    exit(-1);
  }
  node->value = value;
  node -> next = head;
  return node;
}

/* Returns the head node of a new list with the deleted item deleted. */
Node* GENERIC_TYPE_delete(Node* head, Node* delete_node) {
  if (head == NULL || delete_node == NULL) {
    return NULL;
  }
  Node* curr = head;

  if (curr == delete_node) {
    curr = curr->next;
    free(delete_node);
    return curr;
  }
  Node* next = head -> next;
  do {
    if (next == delete_node) {
      curr -> next = next -> next;
      free(next);
      return curr;
    }
    curr = curr -> next;
    next = next -> next;
  } while (NULL != next);

  printf("Called delete on a list that did not containt the target node. Crashing");
  exit(-1);
}
