#ifndef NODE_H__
#define NODE_H__

typedef struct _Node Node;

Node *create_node(void);
Node *destroy_node(Node *node);
void *get_node_data(Node *node);
void fill_node_data(Node *node, unsigned int size, void *data);
Node *get_next_node(Node *node);
Node* append_node(Node* node, unsigned size, void* data);

#endif // NODE_H__
