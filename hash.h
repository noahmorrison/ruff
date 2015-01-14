typedef struct _hash_node_
{
    char key[1024];
    char val[1024];
    struct _hash_node_ *next;
} hash_node;


typedef struct _hash_table_
{
    int size;
    hash_node **data;
} hash_table;


unsigned long	hash(char *);
hash_table	*create_hash_table(int);
int	        ht_insert(hash_table *, char *, char *);
hash_node	*ht_lookup(hash_table *, char *);
