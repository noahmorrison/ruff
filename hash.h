typedef struct _hash_node_
{
    const char *key;
    const char *val;
    struct _hash_node_ *next;
} hash_node;


typedef struct _hash_table_
{
    int size;
    hash_node **data;
} hash_table;


unsigned long hash(char *string);


hash_table *create_hash_table(int size);


void ht_insert(hash_table *table, char *key, char *val);


hash_node *ht_lookup(hash_table *table, char *key);
