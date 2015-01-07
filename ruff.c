#include <stdio.h>
#include <openssl/md5.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>


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


int main(int argc, char **argv)
{
    char *reference;
    DIR *rd;

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

    rd = opendir(reference);
    if (rd == NULL)
    {
        fprintf(stderr, "Could not open reference\n");
        return 2;
    }


    struct dirent *file;
    while ((file = readdir(rd)) != NULL)
    {
        char *name = file->d_name;
        char *path = strcat(reference, name);

        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            continue;

        /* print md5sum and file size */
        char md5[32];
        get_md5(path, md5);

        int size = fsize(path);

        printf("%s: %s - %d\n", path, md5, size);
    }

    return 0;
}
