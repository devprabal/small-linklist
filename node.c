#include "node.h"

static node_data_alloc_func node_data_alloc_fptr     = calloc;
static node_data_dealloc_func node_data_dealloc_fptr = free;

struct Node
{
    void *data;
    Node *next;
};

Node *create_node(void)
{
    Node *node = (Node *)calloc(1, sizeof(Node));
    if (node) return node;
    return NULL;
}

void clear_node_data(Node *node)
{
    if (node) {
        if (node->data) {
            node_data_dealloc_fptr(node->data);
            node->data = NULL;
        }
    }
}

Node *destroy_node(Node *node)
{
    Node *next_node = NULL;
    if (node) {
        next_node = node->next;
        clear_node_data(node);
        free(node);
    }
    return next_node;
}

void *get_node_data(Node *node)
{
    if (node) {
        return node->data;
    }
    return NULL;
}

SLL_Result fill_node_data(Node *node, unsigned int size, void *data)
{
    if (! node || ! data) return SLL_FAIL;
    node->data = (void *)node_data_alloc_fptr(1, size);
    if (node->data) {
        memcpy(node->data, data, size);
        return SLL_SUCCESS;
    }
    return SLL_FAIL;
}

Node *get_next_node(Node *node)
{
    if (node) return node->next;
    return NULL;
}

SLL_Result append_node(Node *node, unsigned size, void *data)
{
    if (node && ! (node->next)) {
        node->next = create_node();
        return fill_node_data(node->next, size, data);
    }
    return SLL_FAIL;
}

void set_node_data_alloc_dealloc_func(node_data_alloc_func alloc_fptr, node_data_dealloc_func dealloc_fptr)
{
    if (alloc_fptr) {
        node_data_alloc_fptr = alloc_fptr;
    }
    if (dealloc_fptr) {
        node_data_dealloc_fptr = dealloc_fptr;
    }
}
