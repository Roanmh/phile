#ifndef DISK_H_
#define DISK_H_

#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define BLOCK_SIZE 4096
#define DISK_SIZE (BLOCK_SIZE * 4096)

#define END_BLOCK_CODE -2
#define FREE_BLOCK_CODE -1
#define SPECIAL_BLOCK_CODE -3
#define DIR_START_BLOCK_INDEX 2
#define FAT_RECORD_BYTE_LEN 2
#define DIR_RECORD_BYTE_LEN 32
#define MAX_FILE_NAME_LEN 26

struct FileRecord {
  char file_name[MAX_FILE_NAME_LEN];
  int16_t start;
  int32_t size;
  int16_t dir_len;
};

int dir_size(const int disk);
int read_file(int disk, struct FileRecord* file);

int16_t first_avail_block(const int disk);
int extend_dir(const int disk);
int get_file_and_start_block(const int disk, const char **write_name,
                             struct FileRecord *file);
int append_poem(const int disk, struct FileRecord *file, char *buffer, int *buff_size);
int mark_end_block(const int disk, int16_t file_index);
int16_t assign_block(const int disk, int16_t file_index);
int clear_file(const int disk, int16_t file_index);
off_t seek_fat_entry(const int disk, const int16_t file_index);
int create_file(const int disk, const char **file_name, struct FileRecord *file);
int format_disk(const int disk);

int verify_disk(int disk, int inv_errs);
void read_fat_table(int disk, int16_t* table);



#endif
