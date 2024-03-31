#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "node.h"

void create_node_tc(void)
{
    Node* node = create_node();
    assert(NULL != node);

    char* node_data = "Chika Home";
    assert(SLL_SUCCESS == fill_node_data(node, strlen(node_data) + 1, node_data));
    char* saved_node_data = (char*)get_node_data(node);

    assert(NULL != saved_node_data);
    assert(strlen(node_data) == strlen(saved_node_data));
    assert(0 == strncmp(saved_node_data, node_data, strlen(node_data)));

    destroy_node(node);
}

typedef struct Person
{
    char* name;
    int age;
} Person;

static void free_person_name(Person* person)
{
    if (person)
    {
        if (person->name)
        {
            free(person->name);
            person->name = NULL;
        }
    }
}

static void user_data_free_func(void* args, void* item)
{
    if (args) {
        size_t* count = (size_t*)args;
        (*count)++;
    }
    free_person_name((Person*)item);
}

static bool compare_person(void* item1, void* item2)
{
    Person* person1 = (Person*)item1;
    Person* person2 = (Person*)item2;
    if (! person1 || ! person2) return false;

    bool is_age_matched = (person1->age == person2->age);

    size_t name1_len    = strlen(person1->name);
    size_t name2_len    = strlen(person2->name);
    if (name1_len != name2_len) return false;

    bool is_name_matched = (0 == strncmp(person1->name, person2->name, name1_len));

    return (is_age_matched && is_name_matched);
}

typedef enum ITEM_TYPE
{
    ITEM_TYPE_PERSON,
    ITEM_TYPE_MAX,
} ITEM_TYPE;

typedef bool (*compare_item_func_t)(void*, void*);

compare_item_func_t compare_item_func_list[ITEM_TYPE_MAX] = {
    [ITEM_TYPE_PERSON] = compare_person,
};

static compare_item_func_t get_compare_item_func(ITEM_TYPE item_type)
{
    return compare_item_func_list[item_type];
}

static size_t make_default_person_list(Head* head)
{
    Person person_list[] = {
        {        .name = strdup("Kei"), .age = 26}, ////TODO: use a combination of malloc and memcpy instead of
  ///  feature_test_macros. strdup is only availabe in c2x std
        {     .name = strdup("Consti"), .age = 16},
        {   .name = strdup("Midnight"), .age = 25},
        {     .name = strdup("Nikhil"), .age = 18},
        {      .name = strdup("Bhumi"), .age = 23},
        {      .name = strdup("Varoo"), .age = 25},
        {      .name = strdup("Luffy"), .age = 19},
        {     .name = strdup("Rodnew"), .age = 27},
        {     .name = strdup("Kelvin"), .age = 12},
        {.name = strdup("CONSTANTINE"), .age = 24},
        {    .name = strdup("devpogi"), .age = 25},
    };

    size_t person_list_len = sizeof(person_list) / sizeof(person_list[0]);
    for (size_t i = 0; i < person_list_len; i++) {
        append_to_list(head, sizeof(Person), &person_list[i]);
    }
    return person_list_len;
}

void create_list_tc(void)
{
    Head* head = create_list(NULL);
    assert(NULL != head);

    size_t default_person_list_len = make_default_person_list(head);
    assert(default_person_list_len == count_nodes(head));

    assert(true ==
           find_in_list(head, &((Person){ .name = "Nikhil", .age = 18 }), get_compare_item_func(ITEM_TYPE_PERSON)));
    assert(false ==
           find_in_list(head, &((Person){ .name = "Nikhil", .age = 28 }), get_compare_item_func(ITEM_TYPE_PERSON)));
    assert(false ==
           find_in_list(head, &((Person){ .name = "Nikii", .age = 18 }), get_compare_item_func(ITEM_TYPE_PERSON)));

    size_t n_person_deleted = 0;
    destroy_list(head, user_data_free_func, &n_person_deleted);
    assert(n_person_deleted == default_person_list_len);
}

#define PERSON_MEM_POOL_SIZE 12
static Person person_memory_pool[PERSON_MEM_POOL_SIZE];
static size_t person_memory_pool_free_idx = 0;

static void* my_list_data_calloc_func(size_t n_items, size_t item_size_bytes)
{
    bool is_one_person_mem_requested = (1 == n_items && sizeof(Person) == item_size_bytes);
    if (! is_one_person_mem_requested) {
        printf(
            "\nMem does not support creating memory for more than 1 person at a time \
            \nor an item other than ITEM_TYPE_PERSON.\n");
        return NULL;
    }
    if (person_memory_pool_free_idx < PERSON_MEM_POOL_SIZE) {
        person_memory_pool_free_idx++;
        return &person_memory_pool[person_memory_pool_free_idx - 1];
    }
    printf("\nPerson mem pool exhausted.\n");
    return NULL;
}

static void my_list_data_free_func(void* item)
{
    free_person_name((Person*)item);
    person_memory_pool_free_idx--;
}

void create_custom_alloc_dealloc_func_tc(void)
{
    Config config = {
        .list_data_alloc_func_fptr   = my_list_data_calloc_func,
        .list_data_dealloc_func_fptr = my_list_data_free_func,
    };
    Head* head = create_list(&config);
    assert(NULL != head);

    size_t default_person_list_len = make_default_person_list(head);
    assert(default_person_list_len == count_nodes(head));

    destroy_list(head,
                 NULL,
                 NULL);  ////TODO: How will user know that once he has set the dealloc func, does he also need to
                         /// pass it here or not? or does he need to pass something else like `user_data_free_func()`?
}

void out_of_mem_tc(void)
{
    Config config = {
        .list_data_alloc_func_fptr   = my_list_data_calloc_func,
        .list_data_dealloc_func_fptr = my_list_data_free_func,
    };
    Head* head = create_list(&config);
    assert(NULL != head);

    size_t default_person_list_len = make_default_person_list(head);
    assert(default_person_list_len == count_nodes(head));

    assert(SLL_SUCCESS ==
           append_to_list(head, sizeof(Person), &(Person){ .name = strdup("Deleted Account1"), .age = 25 }));

    char* name_s = strdup("Deleted Account2");
    assert(SLL_FAIL == append_to_list(head, sizeof(Person), &(Person){ .name = name_s, .age = 25 }));
    free(name_s);
    name_s = NULL;
    destroy_list(head, NULL, NULL);
}

int main(void)
{
    create_node_tc();
    create_list_tc();
    create_custom_alloc_dealloc_func_tc();
    out_of_mem_tc();
    ////TODO: There can be a case when the node creation is success, however, user data fill_node_data fails
    //// Node->Node->Node->Node but all Node->data will be NULL
    //// Test out this case and discuss if it is the correct behaviour, or should be stop making Nodes if Node->data
    /// filling fails? / In that case, we will also need to delete the created Node for which data filling fails
    return 0;
}
