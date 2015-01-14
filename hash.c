#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"


unsigned long
hash(char *s)
{
	char *p;
	unsigned int h, g;

	h = 0;
	for(p=s; *p!='\0'; p++){
		h = (h<<4) + *p;
		if ((g = h) & 0xF0000000) {
			h ^= g>>24;
			h ^= g;
		}
	}
	return h;
}


hash_table *
create_hash_table(int size)
{
	hash_table *table;
	int i;

	if (size < 1)
		return NULL;

	if ((table = malloc(sizeof(struct _hash_table_))) == NULL)
		return NULL;

	if ((table->data = malloc(sizeof(hash_node *) *size)) == NULL)
		return NULL;

	for (i = 0; i < size; i++)
		table->data[i] = NULL;
		table->size = size;

	return table;
}


hash_node *
ht_lookup(hash_table *table, char *key)
{
	unsigned int hashval;
	hash_node *node;

	hashval = (hash(key) % table->size);

	for (node = table->data[hashval]; node != NULL; node = node->next)
		if (strcmp(key, node->key) == 0)
			return node;

	return NULL;
}


int
ht_insert(hash_table *table, char *key, char *val)
{
	unsigned int hashval;
	hash_node *cur_node;
	hash_node *new_node;

	hashval = (hash(key) % table->size);

	if ((new_node = malloc(sizeof(hash_node))) == NULL)
		return -1;

	cur_node = ht_lookup(table, key);

	if (cur_node != NULL)
		return 0;

	strcpy(new_node->key, key);
	strcpy(new_node->val, val);
	new_node->next = table->data[hashval];
	table->data[hashval] = new_node;

	return 1;
}
