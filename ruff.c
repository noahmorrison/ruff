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


int main()
{
    char md5[32];
    get_md5("test", md5);

    int size = fsize("test");

    printf("%s - %d\n", md5, size);

    return 0;
}
