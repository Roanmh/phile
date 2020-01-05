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

/// FAT Codes
// Last Block of file (Also used for directory block)
#define END_BLOCK_CODE -2
// Available to be written to
#define FREE_BLOCK_CODE -1
// FAT Table Block (potential for other special blocks)
#define SPECIAL_BLOCK_CODE -3

// Block/FAT index of first directory entry.
#define DIR_START_BLOCK_INDEX 2
#define FAT_RECORD_BYTE_LEN 2
#define DIR_RECORD_BYTE_LEN 32
#define MAX_FILE_NAME_LEN 26

struct FileRecord {
  char file_name[MAX_FILE_NAME_LEN];
  // FAT index starting block
  int16_t start;
  int32_t size;
  // Location in directory in bytes.
  int16_t dir_loc;
};

// Returns the size of the directory in bytes
// Args:
//   disk: File descriptor of the 'disk'
int dir_size(const int disk);

// Reads File and print to stdout.
// Args:
//   disk: File descriptor of the 'disk'
//   file: FileRecord pointer
// Returns: 0 (future version may include error checking)
int read_file(int disk, struct FileRecord* file);


// Returns the FAT index of the first available block
// Args:
//   disk: File descriptor of the 'disk'
int16_t first_avail_block(const int disk);


// Future Function to create a larger, linked directory.
//int extend_dir(const int disk);

// Finds the file with the name `write_name` and returns its FileRecord.
// Args:
//   disk: File descriptor of the 'disk'
//   write_name: Pointer to the file name to be searched for
//   file: Pointer to be filled with the FileRecord of the matching file
// Effects: Fills `file` with the matching file.
// Returns:
//    0 on success
//   -1 on lseek failure
//   -2 on file not found
int get_file_and_start_block(const int disk, const char **write_name,
                             struct FileRecord *file);
// Appends a poem to the end of an existing file
// Args:
//   disk: File descriptor of the 'disk'
//   file: Pointer to the FileRecord of the file to append to
//   buffer: Pointer to the buffer to be appended to the file
//   buff_size: Pointer to the size of the buffer
int append_poem(const int disk, struct FileRecord *file, char *buffer, int *buff_size);

// Marks the given FAT index as an end block
// Args:
//   disk: File descriptor of the 'disk'
//   file_index: FAT index to be marked as end.
int mark_end_block(const int disk, int16_t file_index);

// Links the given FAT index to the next available block
// Args:
//   disk: File descriptor of the 'disk'
//   file_index: FAT index of the block to be linked
int16_t assign_block(const int disk, int16_t file_index);

// Erase the file beginning at `file_index`
// Args:
//   disk: File descriptor of the 'disk'
//   file_index: FAT index of the block to be erased
int clear_file(const int disk, int16_t file_index);

// Seeks disk to the given FAT entry and the given index
// Returns: lseek result
off_t seek_fat_entry(const int disk, const int16_t file_index);

// Create an empty file at the next available block
// Args:
//   disk: File descriptor of the 'disk'
//   file_name: Pointer to the name of the new file
//   file: Pointer to be filled with the FileRecord of the matching file
int create_file(const int disk, const char **file_name, struct FileRecord *file);

// Erase all disk contents and create emplty FAT and directory.
// Args:
//   disk: File descriptor of the 'disk'
int format_disk(const int disk);

// Read the contents of the FAT into the table pointer
// Args:
//   disk: File descriptor of the 'disk'
//   table: Pointer to a buffer/array to be filled with the FAT.
//          NOTE: Must be BLOCK_SIZE * 2 bytes long. (i.e. int16_t x[BLOCKSIZE])
void read_fat_table(int disk, int16_t* table);



#endif
