#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "cache.h"
#include "print_helpers.h"

cache_t *make_cache(int capacity, int block_size, int assoc, enum protocol_t protocol, bool lru_on_invalidate_f){
  cache_t *cache = malloc(sizeof(cache_t));
  cache->stats = make_cache_stats();
  
  cache->capacity = capacity;      // in Bytes
  cache->block_size = block_size;  // in Bytes
  cache->assoc = assoc;            // 1, 2, 3... etc.

  // FIX THIS CODE!
  // first, correctly set these 5 variables. THEY ARE ALL WRONG
  // note: you may find math.h's log2 function useful
  cache->n_cache_line = capacity/block_size;
  cache->n_set = cache->n_cache_line/assoc;
  cache->n_offset_bit = log2(block_size);
  cache->n_index_bit = log2(cache->n_set);
  cache->n_tag_bit = 32 - cache->n_offset_bit - cache->n_index_bit;

  // next create the cache lines and the array of LRU bits
  // - malloc an array with n_rows
  // - for each element in the array, malloc another array with n_col
  // FIX THIS CODE!

  cache->lines = malloc(cache->n_set*sizeof(cache_line_t *));
  for (int i=0; i<cache->n_set; i++) {
    cache->lines[i] = malloc(cache->assoc*sizeof(cache_line_t)); 
  }
  cache->lru_way = malloc(cache->n_set*sizeof(int));

  // initializes cache tags to 0, dirty bits to false,
  // state to INVALID, and LRU bits to 0
  // FIX THIS CODE!
  for (int i = 0; i < cache->n_set; i++) {
    for (int j = 0; j < cache->assoc; j++) {
      // body goes here
      cache->lines[i][j].tag = 0;
      cache->lines[i][j].dirty_f = false;
      cache->lines[i][j].state = INVALID;
    }
    cache->lru_way[i] = 0;
  }

  cache->protocol = protocol;
  cache->lru_on_invalidate_f = lru_on_invalidate_f;
  
  return cache;
}

/* Given a configured cache, returns the tag portion of the given address.
 *
 * Example: a cache with 4 bits each in tag, index, offset
 * in binary -- get_cache_tag(0b111101010001) returns 0b1111
 * in decimal -- get_cache_tag(3921) returns 15 
 */
unsigned long get_cache_tag(cache_t *cache, unsigned long addr) {
  unsigned long cache_tag = addr >> (cache->n_index_bit + cache->n_offset_bit);
  // FIX THIS CODE!
  return cache_tag;
}

/* Given a configured cache, returns the index portion of the given address.
 *
 * Example: a cache with 4 bits each in tag, index, offset
 * in binary -- get_cache_index(0b111101010001) returns 0b0101
 * in decimal -- get_cache_index(3921) returns 5
 */
unsigned long get_cache_index(cache_t *cache, unsigned long addr) {
  // FIX THIS CODE!
  unsigned long cache_index = (addr >> cache->n_offset_bit) & ~(~0 << cache->n_index_bit);
  return cache_index;
}

/* Given a configured cache, returns the given address with the offset bits zeroed out.
 *
 * Example: a cache with 4 bits each in tag, index, offset
 * in binary -- get_cache_block_addr(0b111101010001) returns 0b111101010000
 * in decimal -- get_cache_block_addr(3921) returns 3920
 */
unsigned long get_cache_block_addr(cache_t *cache, unsigned long addr) {
  // FIX THIS CODE!
  unsigned long cache_block_addr = addr >> cache->n_offset_bit << cache->n_offset_bit;
  return cache_block_addr;
}


/* this method takes a cache, an address, and an action
 * it proceses the cache access. functionality in no particular order: 
 *   - look up the address in the cache, determine if hit or miss
 *   - update the LRU_way, cacheTags, state, dirty flags if necessary
 *   - update the cache statistics (call update_stats)
 * return true if there was a hit, false if there was a miss
 * Use the "get" helper functions above. They make your life easier.
 */
bool access_cache(cache_t *cache, unsigned long addr, enum action_t action) {
  // FIX THIS CODE!
  bool cache_hit = false;
  unsigned long tag = get_cache_tag(cache, addr);
  unsigned long index = get_cache_index(cache, addr);
  unsigned long block_addr = get_cache_block_addr(cache, addr);
  
  int lru = cache->lru_way[index];
  bool writeback_f = cache->lines[index][lru].dirty_f;

  for (int assoc=0; assoc < cache->assoc; assoc++) {
    if (cache->lines[index][assoc].tag == tag && cache->lines[index][assoc].state==VALID) {
      cache_hit = true;
      update_stats(cache->stats, cache_hit, false, false, action);
      // if store -> set dirty bit = true
      if (action==STORE) {
        cache->lines[index][assoc].dirty_f = true;
      }
      // update LRU
      cache->lru_way[index] = (cache->assoc-1) - assoc;
      break;
    }
  }

  if (!cache_hit) {
    // if a miss -> write back + update cache line
    update_stats(cache->stats, cache_hit, writeback_f, false, action);
    cache->lines[index][lru].tag = tag;
    cache->lines[index][lru].state = VALID;
    // set dirty bit store = true; load = false
    if (action==STORE) {
      cache->lines[index][lru].dirty_f = true;
    } else {
      cache->lines[index][lru].dirty_f = false;
    }
    // update LRU
    cache->lru_way[index] = (cache->assoc-1) - lru;
  }

  return cache_hit;  // cache hit should return true
}







