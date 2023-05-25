#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

typedef void (*user_print_func)(void *);
typedef void (*user_free_func)(void *);

typedef struct hash_node_t {
	char *key;
	void *data;
	bool deleted;

} hash_node_t;

typedef struct hashtable_t {
	uint32_t load;
	uint32_t count;
	hash_node_t **buckets;
	pthread_mutex_t table_lock;
	user_print_func print;
	user_free_func destroy;

} hashtable_t;

uint32_t hash_crc32(const char *str);

hashtable_t *create_table(user_print_func print, user_free_func destroy);

void hashtable_destroy(hashtable_t ** table);

bool hashtable_insert(hashtable_t * table, const char *key, void *data);

void hashtable_display(hashtable_t * table);

void * hashtable_lookup(hashtable_t * table, const char *data);

#endif				/* HASHTABLE_H */

/* end of file */
