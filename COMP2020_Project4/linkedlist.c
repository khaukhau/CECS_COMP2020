
#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

struct linkedlist
{
    struct linkedlist_node *first;
    // TODO: define linked list metadata
    int size;
};

struct linkedlist_node
{
    // TODO: define the linked list node
    struct linkedlist_node_t *next;
    int key;
    int value;

};
typedef struct linkedlist_node linkedlist_node_t;

linkedlist_t *ll_init()
{
    // TODO: create a new linked list

    // We have done this TODO for you as an example of how to use malloc().
    // You might want to read more about malloc() from Linux Manual page.
    // Usually free() should be used together with malloc(). For example, 
    // the linkedlist you are currently implementing usually have free() in the
    // ll_delete() function. Since we are not asking you to implement
    // the ll_delete() function, you will not be using free() in this case.

    // First, you use C's sizeof function to determine
    // the size of the linkedlist_t data type in bytes.
    // Then, you use malloc to set aside that amount of space in memory.
    // malloc returns a void pointer to the memory you just allocated, but
    // we can assign this pointer to the specific type we know we created
    linkedlist_t *list = malloc(sizeof(linkedlist_t));

    // TODO: set metadata for your new list and return the new list
    list->size = 0;
    list->first = NULL;
    return list;
}

void ll_add(linkedlist_t *list, int key, int value)
{
    // TODO: create a new node and add to the front of the linked list if a
    // node with the key does not already exist.
    // Otherwise, replace the existing value with the new value.
    linkedlist_node_t *k = list->first;
    if (k == NULL){
        k = malloc(sizeof(linkedlist_node_t));
        k->key = key;
        k->value = value;
        k->next = NULL;
        list->first = k;
        list->size++;
        return;
    }
    else {
        while (k->next != NULL){
            if (k->key != key){
                k = k->next;
            }
            else{
                k->value = value;
                return;
            }
        }
         if (k->key == key){
             k->value = value;
            return;
        }
        k->next = malloc(sizeof(linkedlist_node_t));
        k = k->next;
        k->key = key;
        k->value = value;
        list->size++;
        return;
        }
    }
int ll_get(linkedlist_t *list, int key)
{
    // TODO: go through each node in the linked list and return the value of
    // the node with a matching key.
    // If it does not exist, return 0.
    linkedlist_node_t *t = list->first;
    while (t != NULL)
    {
        if (t->key != key){
            t = t->next;
        }
        else
        {
            return t->value;
        }
    }
    return 0;
}
int ll_size(linkedlist_t *list)
{
    // TODO: return the number of nodes in this linked list
    return list->size;
}
