#ifndef LIST_H__
#define LIST_H__

#include "node.h"

typedef struct Head Head;
typedef node_data_alloc_func list_data_alloc_func;
typedef node_data_dealloc_func list_data_dealloc_func;
typedef struct Config
{
    list_data_alloc_func list_data_alloc_func_fptr;
    list_data_dealloc_func list_data_dealloc_func_fptr;
} Config;

typedef void (*list_func)(void*, void*);

Head* create_list(Config* config);
void destroy_list(Head* head, void (*destroy_user_data_func)(void*, void*), void* user_args);
unsigned count_nodes(Head* head);
SLL_Result append_to_list(Head* head, unsigned size, void* data);
bool find_in_list(Head* head, void* item, bool (*compare_func)(void*, void*));
void set_config(Config* config);

#endif  // LIST_H__
