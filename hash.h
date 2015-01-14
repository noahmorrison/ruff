typedef struct _hash_node_ {
    struct _hash_node_	*next;
    char	        key[1024];
    char	        val[1024];
} hash_node;


typedef struct _hash_table_ {
    hash_node	**data;
    int	        size;
} hash_table;


unsigned long	hash(char *);
hash_table	*create_hash_table(int);
int	        ht_insert(hash_table *, char *, char *);
hash_node	*ht_lookup(hash_table *, char *);
