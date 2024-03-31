#ifndef NODE_H__
#define NODE_H__

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node Node;
typedef Node*(* node_func)(Node*);
typedef void* (*node_data_alloc_func)(size_t n_items, size_t item_size_bytes);
typedef void (*node_data_dealloc_func)(void* item);

typedef enum SLL_Result {
    SLL_SUCCESS,
    SLL_FAIL
} SLL_Result;

Node *create_node(void);
Node *destroy_node(Node *node);
void *get_node_data(Node *node);
SLL_Result fill_node_data(Node *node, unsigned int size, void *data);
void clear_node_data(Node *node);
Node *get_next_node(Node *node);
SLL_Result append_node(Node* node, unsigned size, void* data);
void set_node_data_alloc_dealloc_func(node_data_alloc_func alloc_fptr, node_data_dealloc_func dealloc_fptr);

#endif // NODE_H__
