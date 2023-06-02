#include <assert.h>
#include <stdint.h>

#include "node.h"
#include "list.h"

void create_node_tc(void)
{
    Node *node = create_node();
    assert(NULL != node);

    char *node_data = "kei senpai";
    fill_node_data(node, strlen(node_data)+1, node_data);
    char *saved_node_data = (char *)get_node_data(node);
    
    assert(NULL != saved_node_data);
    assert(strlen(node_data) == strlen(saved_node_data));
    assert(0 == strncmp(saved_node_data, node_data, strlen(node_data)));
    
    destroy_node(node);
}

typedef struct Person {
    char* name;
    int age;
} Person;

static void free_person_name(Person* person)
{
    if(person)
    {
        if(person->name)
        {
            free(person->name);
            person->name = NULL;
        }
    }
}

static void* user_node_data_free_func(void* item)
{
    free_person_name((Person*)item);
    return NULL;
}

static bool compare_person(Person* person1, Person* person2)
{
    if(!person1 || !person2) return false;

    bool is_age_matched = (person1->age == person2->age);

    size_t name1_len = strlen(person1->name);
    size_t name2_len = strlen(person2->name);
    if(name1_len != name2_len) return false;

    bool is_name_matched = (0 == strncmp(person1->name, person2->name, name1_len));

    return (is_age_matched && is_name_matched);
}

typedef enum ITEM_TYPE {
   ITEM_TYPE_PERSON,
   ITEM_TYPE_MAX,
} ITEM_TYPE;

typedef bool(*compare_item_func_t)(void*, void*);

compare_item_func_t compare_item_func_list[ITEM_TYPE_MAX] = {
    [ITEM_TYPE_PERSON] = compare_person,
};

static compare_item_func_t get_compare_item_func(ITEM_TYPE item_type)
{
    return compare_item_func_list[item_type];
}

void create_list_tc(void)
{
    Head* head = create_list();
    assert(NULL != head);

    Person person_list[] = {
        {.name = strdup("Kei"), .age = 26},
        {.name = strdup("Consti"), .age = 16},
        {.name = strdup("Midnight"), .age = 25},
        {.name = strdup("Nikhil"), .age = 18},
        {.name = strdup("Bhumi"), .age = 23},
        {.name = strdup("Varoo"), .age = 25},
        {.name = strdup("Luffy"), .age = 19},
        {.name = strdup("Rodnew"), .age = 27},
        {.name = strdup("Kelvin"), .age = 12},
    };

    uint8_t person_list_len = sizeof(person_list)/sizeof(person_list[0]);
    for(uint8_t i = 0; i < person_list_len; i++)
    {
        append_to_list(head, sizeof(Person), &person_list[i]);
    }

    assert(person_list_len == count_nodes(head));

    assert(true == find_in_list(head, &((Person){.name = "Nikhil", .age = 18}), get_compare_item_func(ITEM_TYPE_PERSON)));
    assert(false == find_in_list(head, &((Person){.name = "Nikhil", .age = 28}), get_compare_item_func(ITEM_TYPE_PERSON)));
    assert(false == find_in_list(head, &((Person){.name = "Nikii", .age = 18}), get_compare_item_func(ITEM_TYPE_PERSON)));

    set_user_data_free_func(user_node_data_free_func);
    destroy_list(head);
}

int main(void)
{
    create_node_tc();
    create_list_tc();
    return 0;
}
