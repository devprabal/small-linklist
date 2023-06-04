#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "list.h"

typedef struct Person
{
    char *name;
    int age;
} Person;

typedef _Bool (*compare_item_func_t)(Person *, Person *);

_Bool compare_person(Person *person1, Person *person2)
{
    return person1->age < person2->age;
}

#define NUM_ITEM_TYPES 1
#define ITEM_TYPE_PERSON 0

compare_item_func_t compare_item_func_list[NUM_ITEM_TYPES] = {compare_person};

void create_node_tc(void)
{
    Node *node = create_node();
    assert(NULL != node);

    char *node_data = "kei senpai";
    fill_node_data(node, strlen(node_data) + 1, node_data);
    char *saved_node_data = (char *)get_node_data(node);

    assert(NULL != saved_node_data);
    assert(strlen(node_data) == strlen(saved_node_data));
    assert(0 == strncmp(saved_node_data, node_data, strlen(node_data)));

    destroy_node(node);
}

static void free_person_name(Person *person)
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

static void *user_node_data_free_func(void *item)
{
    free_person_name((Person *)item);
    return NULL;
}

void create_list_tc(void)
{
    Head *head = create_list();
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

    uint8_t person_list_len = sizeof(person_list) / sizeof(person_list[0]);
    for (uint8_t i = 0; i < person_list_len; i++)
    {
        append_to_list(head, sizeof(Person), &person_list[i]);
    }

    assert(person_list_len == count_nodes(head));

    set_user_data_free_func(user_node_data_free_func);

    compare_item_func_list[ITEM_TYPE_PERSON] = compare_person;

    destroy_list(head);
}

int main(void)
{
    create_node_tc();
    create_list_tc();
    return 0;
}
