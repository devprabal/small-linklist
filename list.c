#include "list.h"

struct Head {
    Node* front;
};

static void iterate_list(Head* head, node_func node_fptr, list_func user_fptr, void* user_func_args)
{
    if(!node_fptr) {
        printf("\nNeed to provide function for node iteration\n");
        node_fptr = get_next_node;
    }
    
    if(head) {
        Node* front = head->front;
        while(front) {
            void* node_data = get_node_data(front);
            if(user_fptr) {
                user_fptr(user_func_args, node_data);
            }
            front = node_fptr(front); 
        }
    }
}

static void increment(void* args, void* Node_data)
{
    if(!args) {
        printf("\nPass unsigned type output storage\n");
    }
    if(!Node_data)
    {
        printf("\nNode data does not exist\n");
    }
    unsigned *count = (unsigned *)args;
    if(count) {
        ++(*count);
    }
}

Head* create_list(Config* config)
{
    set_config(config);
    Head* head = calloc(1, sizeof(Head));
    if(head) return head;
    return NULL;
}

void destroy_list(Head* head, void(* destroy_user_data_func)(void*, void*), void* user_args)
{
    iterate_list(head, destroy_node, destroy_user_data_func, user_args);
    free(head);
}

unsigned count_nodes(Head* head)
{
    unsigned count = 0;
    iterate_list(head, get_next_node, increment, &count);
    return count;
}

SLL_Result append_to_list(Head* head, unsigned size, void* data)
{
    if (!head)
    {
        return SLL_FAIL;
    }

    if (!(head->front))
    {
        head->front = create_node();
        return fill_node_data(head->front, size, data);
    }
    else
    {
        Node* this_node = head->front;
        Node* next_node = get_next_node(this_node);
        while (next_node)
        {
            this_node = next_node;
            next_node = get_next_node(next_node);
        }
        return append_node(this_node, size, data);
    }
}

bool find_in_list(Head* head, void* item, bool(* compare_func)(void*, void*))
{
    if(!compare_func)
    {
        printf("\nProvide function for data comparison\n");
        return false;
    }
    if(head) {
        Node* front = head->front;
        while(front) {
            Node* node_data = get_node_data(front);
            if(compare_func(item, node_data))
            {
                return true;
            }
            front = get_next_node(front);
        }
    }
    return false;
}

void set_config(Config* config)
{
    if(config && (config->list_data_alloc_func_fptr || config->list_data_dealloc_func_fptr))
    {
        if(NULL == config->list_data_alloc_func_fptr)
        {
            printf("\nProvide function for data allocation. Aborting.\n");
            exit(EXIT_FAILURE);
        }
        if(NULL == config->list_data_dealloc_func_fptr)
        {
            printf("\nProvide function for data deallocation. Aborting.\n");
            exit(EXIT_FAILURE);
        }
        set_node_data_alloc_dealloc_func(config->list_data_alloc_func_fptr, config->list_data_dealloc_func_fptr);
    }
    else
    {
        printf("\nProvide functions for data allocation/dealloc. Using calloc() and free() by default\n");
    }
}
