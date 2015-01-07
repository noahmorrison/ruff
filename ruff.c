#include <stdio.h>
#include <openssl/md5.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#include "hash.h"


void get_md5(const char *filename, char str[])
{
    /* open file */
    FILE *fp;

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "File does not exist: %s", filename);
    }

    /* generate md5sum*/
    MD5_CTX ctx;
    ssize_t bytes;
    unsigned char data[1024];

    MD5_Init(&ctx);
    while ((bytes = fread(data, 1, 1024, fp)) != 0)
        MD5_Update(&ctx, data, bytes);
    MD5_Final(data, &ctx);

    /* convert to hex string */
    int i;
    for (i = 0; i < MD5_DIGEST_LENGTH; i++)
        sprintf(&str[i * 2], "%02x", data[i]);
}


void get_size(const char *filename, char result[])
{
    struct stat st;

    if (stat(filename, &st) == 0)
        sprintf(result, "%u", (unsigned int)st.st_size);
}


void print_usage(char *binary)
{
    printf("Usage: %s reference\n", binary);
}


void map_dir(hash_table *results, void(*func)(const char *, char []), char *dir_path)
{
    DIR *dir;

    dir = opendir(dir_path);

    if (dir == NULL)
    {
        fprintf(stderr, "Could not open %s as a directory", dir_path);
        return;
    }

    struct dirent *file;
    while ((file = readdir(dir)) != NULL)
    {
        char *name = file->d_name;
        char path[PATH_MAX];
        path[0] = '\0';

        strcat(path, dir_path);
        strcat(path, name);

        printf("path is %s\n   dir_path is %s\n\n", path, dir_path);

        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            continue;

        char result[NAME_MAX];
        func(path, result);
        ht_insert(results, result, path);
    }
}


int main(int argc, char **argv)
{
    char *reference;

    /* parse arguments */
    if (argc != 2)
    {
        print_usage(argv[0]);
        return 1;
    }

    reference = argv[1];

    if (reference == NULL)
    {
        fprintf(stderr, "No reference given\n");
        return 1;
    }


    hash_table *sizes = create_hash_table(1024);
    map_dir(sizes, get_size, reference);

    hash_node *node = ht_lookup(sizes, "14");
    printf("%s\n", node->val);

    return 0;
}
