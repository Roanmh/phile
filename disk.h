#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int dir_size(const int disk);
int16_t first_avail_block(const int disk);
int extend_dir(const int disk);
int get_file_index(const int disk, const char **file_name);
int append_poem(const int disk, int16_t file_index, char *buffer);
int mark_end_block(const int disk, int16_t file_index);
int16_t assign_block(const int disk, int16_t file_index);
int clear_file(const int disk, int16_t file_index);
off_t seek_fat_entry(const int disk, const int16_t file_index);
int create_file(const int disk, const char **file_name);
int create_file(const int disk, const char **file_name);
int format_disk(const int disk);

int verify_disk(int disk, int inv_errs);
