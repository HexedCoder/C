#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "hashtable.h"

#define LOAD_FACTOR 8
#define MAX_LOAD .75

hashtable_t *create_table(user_print_func print, user_free_func destroy)
{

	hashtable_t *table = calloc(1, sizeof(hashtable_t));
	if (!table) {
		perror("hashtable calloc");
		errno = 0;
		return NULL;
	}

	table->load = LOAD_FACTOR;
	table->count = 0;

	// allocate memory for initial entries array
	table->buckets = calloc(table->load, sizeof(hash_node_t *));
	if (!table->buckets) {
		perror("hashtable buckets calloc");
		errno = 0;
		free(table);
		return NULL;
	}
	// initialize table lock
	if (0 != pthread_mutex_init(&table->table_lock, NULL)) {
		perror("pthread initialization");
		free(table->buckets);
		free(table);
		return NULL;
	}

	table->print = print;
	table->destroy = destroy;

	return table;
}

static void destroy_hashtable_entry(hash_node_t * entry, user_free_func destroy)
{
	if (!entry) {
		return;
	}

	if (destroy && entry->data) {
		destroy(entry->data);
	}
	free(entry->key);
	free(entry);
	entry = NULL;
}

void hashtable_destroy(hashtable_t ** table)
{
	if (!table || !(*table)) {
		return;
	}

	hashtable_t *t = *table;

	pthread_mutex_lock(&t->table_lock);

	// free buckets
	for (uint64_t i = 0; i < t->load; ++i) {
		hash_node_t *entry = t->buckets[i];
		destroy_hashtable_entry(entry, t->destroy);
	}

	// free buckets array
	free(t->buckets);

	// destroy table lock
	pthread_mutex_unlock(&t->table_lock);
	pthread_mutex_destroy(&t->table_lock);

	free(t);
	*table = NULL;

}

static void pack_new_table(hashtable_t ** old_table)
{
	uint32_t curr_size = (*old_table)->load;

	hashtable_t temp_table = { 0 };
    temp_table.load = curr_size * 2;
	
	(*old_table)->load *= 2;

	hash_node_t **new_buckets =
	    calloc((*old_table)->load, sizeof(hash_node_t *));

	if (!new_buckets) {
		perror("hashtable realloc");
		errno = 0;
		(*old_table)->load /= 2;
		fprintf(stderr, "insertion failed\n");
		return;
	}

    temp_table.buckets = new_buckets;

	for (uint64_t i = 0; i < (*old_table)->load / 2; ++i) {
		if ((*old_table)->buckets[i] && (*old_table)->buckets[i]->data
		    && !(*old_table)->buckets[i]->deleted) {

			hashtable_insert(&temp_table, (*old_table)->buckets[i]->key,
					 (*old_table)->buckets[i]->data);

            destroy_hashtable_entry((*old_table)->buckets[i], NULL);
		}
    }

    free((*old_table)->buckets);
	(*old_table)->buckets = temp_table.buckets;


}

bool hashtable_insert(hashtable_t * table, const char *key, void *data)
{
	if (!table || !key) {
		return false;
	}
    
	if (((float)(table->count + 1) / table->load) > MAX_LOAD) {
		pack_new_table(&table);
	}

	uint32_t hashed = hash_crc32(key);
	uint32_t index = hashed % (table->load);


	// check for collision
	while (NULL != table->buckets[index]) {
		// handle collision by linear probing (incrementing index)
		index = (index + 1) % table->load;
	}

	// create new entry
	hash_node_t *new_entry = calloc(1, sizeof(hash_node_t));
	new_entry->key = strdup(key);
	new_entry->data = data;
	new_entry->deleted = false;

	// insert entry
	table->buckets[index] = new_entry;
	++table->count;
	return true;
}

void * hashtable_lookup(hashtable_t * table, const char *key) {
	if (!table || !key) {
		return NULL;
	}

	uint32_t hashed = hash_crc32(key);
	uint32_t index = hashed % (table->load);

	// check for key
	while (NULL != table->buckets[index]) {
		// find key
		if (!strcmp(table->buckets[index]->key, key)) {
	    		return table->buckets[index]->data;
		}

		index = (index + 1) % table->load;
	}
	// not found
	return NULL;
}

void hashtable_display(hashtable_t * table)
{
	if (!table) {
		return;
	}

	user_print_func display = table->print;

	printf("\n----- HASH TABLE -----\n\n");
	for (uint32_t i = 0; i < table->load; ++i) {
		printf("[%d]: ", i);

		if (table->buckets[i]) {
			display(table->buckets[i]->data);
		} else {
			puts("");
		}
	}
	printf("\n--------- END ---------\n");

}

/**
 * @brief calculates the CRC-32 based on table in function.
 * 
 * @cite adapted from https://web.mit.edu/freebsd/head/sys/libkern/crc32.c
 * 
 * @param str 
 * @return uint32_t 
 */
uint32_t hash_crc32(const char *str)
{
	size_t length = strlen(str);
	const uint8_t *data = (const uint8_t *)str;

	uint32_t crc = 0xFFFFFFFF;

	static const uint32_t crc32Table[256] = {
		0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419,
		0x706AF48F, 0xE963A535, 0x9E6495A3,
		0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B,
		0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
		0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D,
		0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
		0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F,
		0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
		0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1,
		0x4B04D447, 0xD20D85FD, 0xA50AB56B,
		0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3,
		0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
		0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5,
		0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
		0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87,
		0x58684C11, 0xC1611DAB, 0xB6662D3D,
		0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589,
		0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
		0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB,
		0x086D3D2D, 0x91646C97, 0xE6635C
	};

	for (size_t i = 0; i < length; ++i) {
		uint8_t index = (crc ^ data[i]) & 0xFF;
		crc = (crc >> 8) ^ crc32Table[index];
	}

	return crc ^ 0xFFFFFFFF;
}