#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "hashtable.h"

struct hashtable {
    // TODO: define hashtable struct to use linkedlist as buckets
    linkedlist_t **bucket;
    int size;
    int max_range;
};

  //Hash function to hash a key into the range [0, max_range)
static int hash(int key, int max_range) {
    // TODO: feel free to write your own hash function (NOT REQUIRED)
    key = (key > 0) ? key : -key;
    return key % max_range;
}

hashtable_t *ht_init(int num_buckets) {
    // TODO: create a new hashtable
    hashtable_t* ht = malloc(sizeof(hashtable_t));
    ht->bucket = malloc(sizeof(linkedlist_t*) * num_buckets);
    ht->size = 0;
    ht->max_range = num_buckets;
    return ht;
}

void ht_add(hashtable_t *table, int key, int value) {
    // TODO: create a new mapping from key -> value.
    // If the key already exists, replace the value.
    int hash_key = hash(key, table->max_range);
    //linkedlist_t* tmp = table->bucket[hash_key];
    //linkedlist_node_t *tmp_node = tmp -> first;

    if (table->bucket[hash_key] == NULL){
        table->bucket[hash_key]=ll_init();
     }
        int before = ll_size(table->bucket[hash_key]);
        ll_add(table->bucket[hash_key],key,value);
        int current = ll_size(table->bucket[hash_key]);
        if (current != before){
            table->size++; 
        }
    
}

int ht_get(hashtable_t *table, int key) {
    // TODO: retrieve the value mapped to the given key.
    // If it does not exist, return 0
    int hash_key = hash(key, table->max_range);
    if (table->bucket[hash_key] == NULL){
        return 0;
    }
    else{
        return ll_get(table->bucket[hash_key],key);
    }
}

int ht_size(hashtable_t *table) {
    // TODO: return the number of mappings in this hashtable
    return table->size;
}
