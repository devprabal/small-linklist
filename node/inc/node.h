#ifndef __NODE_H__
#define __NODE_H__

typedef struct _Node Node;

Node *create_node(void);
Node *destroy_node(Node *node);
void *get_node_data(Node *node);
void fill_node_data(Node *node, unsigned int size, void *data);
Node *get_next_node(Node *node);

#endif // __NODE_H__
