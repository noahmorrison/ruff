#define MAX_DIR 16

#include <sys/stat.h>

#include <dirent.h>
#include <getopt.h>
#include <openssl/md5.h>
#include <stdio.h>
#include <string.h>

#include "hash.h"


void	get_md5(const char *, char []);
void	get_size(const char*, char []);
void	print_usage(char *);
void	map_dir(hash_table *, void(*)(const char *, char []), char *);
int	main(int, char **);


void
get_md5(const char *filename, char str[])
{
	/* open file */
	FILE *fp;

	fp = fopen(filename, "r");

	if (fp == NULL)
		fprintf(stderr, "File does not exist: %s", filename);

	/* generate md5sum*/
	unsigned char data[1024];
	MD5_CTX ctx;
	ssize_t bytes;

	MD5_Init(&ctx);
	while ((bytes = fread(data, 1, 1024, fp)) != 0)
		MD5_Update(&ctx, data, bytes);
	MD5_Final(data, &ctx);

	/* convert to hex string */
	int i;
	for (i = 0; i < MD5_DIGEST_LENGTH; i++)
		sprintf(&str[i * 2], "%02x", data[i]);
}


void
get_size(const char *filename, char result[])
{
	struct stat st;

	if (stat(filename, &st) == 0)
		sprintf(result, "%u", (unsigned int)st.st_size);
}


void
print_usage(char *binary)
{
	printf("Usage: %s reference dup\n", binary);
}


void
map_dir(hash_table *results, void(*func)(const char *, char []), char *dir_path)
{
	DIR *dir;

	dir = opendir(dir_path);

	if (dir == NULL) {
		fprintf(stderr, "Could not open %s as a directory", dir_path);
		return;
	}

	struct dirent *file;
	while ((file = readdir(dir)) != NULL) {
		struct stat st;
		char result[NAME_MAX];
		char path[PATH_MAX];
		char *name;

		name = file->d_name;
		path[0] = '\0';
		result[0] = '\0';

		if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
			continue;

		strcat(path, dir_path);
		strcat(path, "/");
		strcat(path, name);

		stat(path, &st);
		if (S_ISDIR(st.st_mode)) {
			map_dir(results, func, path);
			continue;
		}

		func(path, result);

		if (ht_insert(results, result, path) == 0)
			printf("    collision at %s: %s\n", path, result);
		else
			printf("    inserted %s: %s\n", path, result);
	}
}


int
main(int argc, char **argv)
{
	char dir_paths[MAX_DIR][PATH_MAX];
	int dir_modes[MAX_DIR];
	int dir_count;

	/* parse arguments */
	char ch;
	dir_count = 0;
	while ((ch = getopt(argc, argv, "r:n:")) != -1) {
		if (dir_count >= MAX_DIR) {
			fprintf(stderr, "%d is too many directories (max: %d)\n", dir_count, MAX_DIR);
			return 1;
		}

		switch (ch) {
		case 'r':
			strcpy(dir_paths[dir_count], optarg);
			dir_modes[dir_count] = 0;
			dir_count++;
			break;
		case 'n':
			strcpy(dir_paths[dir_count], optarg);
			dir_modes[dir_count] = 1;
			dir_count++;
			break;
		}
	}

	hash_table *sizes = create_hash_table(1024);

	char *dir;
	int mode;
	int i;
	for (i = 0; i < dir_count; i++) {
		dir = dir_paths[i];
		mode = dir_modes[i];
		printf("dir (mode %d): %s\n", mode, dir);
		map_dir(sizes, get_size, dir);
	}

	return 0;
}
