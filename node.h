#ifndef NODE_H__
#define NODE_H__

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node Node;
typedef Node*(* node_func)(Node*);
typedef void* (* user_data_free_func)(void*);

Node *create_node(void);
Node *destroy_node(Node *node);
void *get_node_data(Node *node);
void fill_node_data(Node *node, unsigned int size, void *data);
Node *get_next_node(Node *node);
Node* append_node(Node* node, unsigned size, void* data);
void set_user_data_free_func(user_data_free_func user_fptr);

#endif // NODE_H__
