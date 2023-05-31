#ifndef LIST_H__
#define LIST_H__

#include "node.h"

typedef struct Head Head;
typedef void(* list_func)(void*);

Head* create_list(void);
void destroy_list(Head* head);
unsigned count_nodes(Head* head);
int append_to_list(Head* head, unsigned size, void* data);

#endif // LIST_H__