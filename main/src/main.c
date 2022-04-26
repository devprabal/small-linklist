#include <stdio.h>
#include <string.h>
#include "node/inc/node.h"

int main(void)
{
    Node *node = create_node();
    char *node_data = "hi";
    fill_node_data(node, strlen(node_data)+1, node_data);
    char *saved_node_data = (char *)get_node_data(node);
    printf("Saved Node Data is %s\n", saved_node_data);
    destroy_node(node);
    return 0;
}
