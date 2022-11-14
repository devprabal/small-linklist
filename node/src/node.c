#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node/inc/node.h"

struct _Node 
{
    void *data;
    Node *next;
};

Node *create_node(void)
{
    Node *node = (Node *)calloc(1, sizeof(Node));
    if(node) return node;
    return NULL;
}

Node *destroy_node(Node *node)
{
    Node *next_node = NULL;
    if(node)
    {
        next_node = node->next;
        if(node->data) free(node->data);
        free(node);
    }
    return next_node;
}

void *get_node_data(Node *node)
{
    if(node)
    {
        return node->data;
    }
    return NULL;
}

void fill_node_data(Node *node, unsigned int size, void *data)
{
    if(!node || !data) return;
    node->data = (void *)malloc(size);
    if(node->data)
    {
        memcpy(node->data, data, size);
    }
}

Node *get_next_node(Node *node)
{
    if(node) return node->next;
    return NULL;
}

Node* append_node(Node* node, unsigned size, void* data)
{
    if(node && !(node->next)) {
        node->next = create_node();
        fill_node_data(node->next, size, data);
        return node->next;
    }
    return NULL;
}
