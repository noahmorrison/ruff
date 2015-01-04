#include <stdio.h>
#include <openssl/md5.h>
#include <sys/stat.h>


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


int fsize(const char *filename)
{
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1;
}


void print_usage(char *binary)
{
    printf("Usage: %s reference_file\n", binary);
}


int main(int argc, char **argv)
{
    char *filename;

    /* parse arguments */
    if (argc != 2)
    {
        print_usage(argv[0]);
        return 1;
    }

    filename = argv[1];

    if (filename == NULL)
    {
        fprintf(stderr, "No reference file given\n");
        return 1;
    }

    /* print md5sum and file size */
    char md5[32];
    get_md5(filename, md5);

    int size = fsize(filename);

    printf("%s - %d\n", md5, size);

    return 0;
}
