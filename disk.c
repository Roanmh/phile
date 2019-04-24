// Functions related to the basic operations of the filesystem.

#include "disk.h"

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
  int size;
};

int dir_size(const int disk) {
  int entry_cnt = 0;
  int16_t block_ind = DIR_START_BLOCK_INDEX;
  int16_t start;
  off_t disk_pos;

  while (block_ind != -2) {
    printf("Active block: %i\n", block_ind);

    disk_pos = lseek(disk, block_ind * BLOCK_SIZE + offsetof(struct FileRecord, start), SEEK_SET);
    read(disk, &start, sizeof(start));
    printf("First file start block: %i @ %li\n", start, disk_pos);

    // TODO: Support linked dir.
    while (start > DIR_START_BLOCK_INDEX /* && entry_cnt % 4096 != 0 */) {
      entry_cnt++;

      disk_pos = lseek(disk, DIR_RECORD_BYTE_LEN - sizeof(start), SEEK_CUR);
      read(disk, &start, sizeof(start));
      printf("%indrdth file start block: %i @ %li\n", entry_cnt + 1, start, disk_pos);
    }

    lseek(disk, block_ind * FAT_RECORD_BYTE_LEN, SEEK_SET);
    read(disk, &block_ind, sizeof(start));
  }

  printf("Entry Count: %i\n", entry_cnt);
  return entry_cnt * DIR_RECORD_BYTE_LEN;
}

int16_t first_avail_block(const int disk) {
  int16_t fat_entry;
  lseek(disk, 0, SEEK_SET);
  for (int i = 0; i < BLOCK_SIZE * 2; i++) {
    read(disk, &fat_entry, sizeof(fat_entry));
    if (fat_entry == FREE_BLOCK_CODE) return i;
  }

  printf("Failed to wrtie: disk full!");
  return -1;
}

// TODO Support linked dir.
int extend_dir(const int disk) {
  return 0;
}

int read_file(const int disk, int16_t file_index)

int get_file_index(const int disk, const char **write_name) {
  char existing_name[MAX_FILE_NAME_LEN];
  lseek(disk, DIR_START_BLOCK_INDEX * 4096, SEEK_SET);

  // TODO Support linked dir
  for (int i = 0; i < BLOCK_SIZE / DIR_RECORD_BYTE_LEN; i++) {
    read(disk, &existing_name, MAX_FILE_NAME_LEN);
    if (!strcmp(existing_name, *write_name)) {
      int16_t file_index;
      // After reading the file name the 'disk head' is at the start value.
      read(disk, &file_index, sizeof(file_index));
      return file_index;
    }
    lseek(disk, DIR_RECORD_BYTE_LEN - MAX_FILE_NAME_LEN, SEEK_CUR);
  }
  return -1;
}

int append_poem(const int disk, int16_t file_index, char *buffer) {
  off_t disk_pos;

  // TODO Update length in directory entry.
  /* int length; */
  /* lseek(disk, DIR_START_BLOCK_INDEX * BLOCK_SIZE + ) */

  // TODO Seek to end of file

  while (strlen(buffer)) {
    // Seek to block
    disk_pos = lseek(disk, file_index * BLOCK_SIZE, SEEK_SET);

    // Write rest of buffer or 4096 bytes and move buffer pointer
    // TODO Support appending in middle of block
    int write_len = strlen(buffer) < BLOCK_SIZE ? strlen(buffer) : BLOCK_SIZE;
    write(disk, buffer, write_len);
    buffer += write_len;

    // If more bytes in buffer: allocate new block
    if (strlen(buffer)) {
      file_index = assign_block(disk, file_index);
    }
  }

  mark_end_block(disk, file_index);
}

int mark_end_block(const int disk, int16_t file_index) {
  int16_t end_block_code = END_BLOCK_CODE;
  lseek(disk, file_index * FAT_RECORD_BYTE_LEN, SEEK_SET);
  write(disk, &end_block_code, sizeof(end_block_code));
  return 0;
}

int16_t assign_block(const int disk, int16_t file_index) {
  int16_t block_addr = first_avail_block(disk);

  // Seek to FAT entry
  seek_fat_entry(disk, file_index);

  // assign first available block and return file_index
  write(disk, &block_addr, sizeof(block_addr));
  return block_addr;
}

off_t seek_fat_entry(const int disk, const int16_t file_index) {
  return lseek(disk, file_index * FAT_RECORD_BYTE_LEN, SEEK_SET);
}

int clear_file(const int disk, int16_t file_index) {
  const int8_t empty_block[4096] = {0};
  const int16_t end_block_code = END_BLOCK_CODE;
  const int16_t free_block_code = FREE_BLOCK_CODE;
  off_t disk_pos;

  // Clear file size.
  disk_pos = lseek(disk, DIR_START_BLOCK_INDEX * BLOCK_SIZE
                   + file_index * DIR_RECORD_BYTE_LEN
                   + MAX_FILE_NAME_LEN
                   + sizeof(int16_t), SEEK_SET);
  write(disk, &empty_block, sizeof(int32_t));

  // Clear First block
  disk_pos = lseek(disk, file_index * BLOCK_SIZE, SEEK_SET);
  write(disk, &empty_block, BLOCK_SIZE);

  // Assign as end block in FAT table
  disk_pos = lseek(disk, file_index * FAT_RECORD_BYTE_LEN, SEEK_SET);
  read(disk, &file_index, sizeof(file_index)); // Read next file index
  disk_pos = lseek(disk, sizeof(file_index) * -1, SEEK_CUR); // Seek back to same FAT entry
  write(disk, &end_block_code, sizeof(end_block_code));

  while (file_index != FREE_BLOCK_CODE) {
    // Free allocation in FAT table
    disk_pos = lseek(disk, file_index * FAT_RECORD_BYTE_LEN, SEEK_SET);
    write(disk, &free_block_code, sizeof(free_block_code));

    // Assign as end block in FAT table
    disk_pos = lseek(disk, file_index * FAT_RECORD_BYTE_LEN, SEEK_SET);
    read(disk, &file_index, sizeof(file_index)); // Read next file index
    disk_pos = lseek(disk, sizeof(file_index) * -1, SEEK_CUR); // Seek back to same FAT entry
    write(disk, &end_block_code, sizeof(end_block_code));

  }

}

int create_file(const int disk, const char **file_name) {
  int16_t entry_offset = dir_size(disk);
  int16_t avail_block_ind = first_avail_block(disk);
  if (avail_block_ind == -1) return -1;
  int16_t end_block_code = END_BLOCK_CODE;
  int32_t new_file_size = 0;
  off_t disk_pos;

  // TODO Support linked dir.
  // Check size for expansion
  /* if (dir_size(disk) / LOCK_SIZE >= 1) { */
  /*   extend_dir(disk); */
  /* } */

  // Find first available dir entry
  disk_pos = lseek(disk, DIR_START_BLOCK_INDEX * BLOCK_SIZE + entry_offset, SEEK_SET);


  // Write file_name, assign first block, size=0
  int name_len = strlen(*file_name) < MAX_FILE_NAME_LEN? strlen(*file_name) : MAX_FILE_NAME_LEN;
  write(disk, *file_name, name_len);
  disk_pos = lseek(disk, DIR_START_BLOCK_INDEX * BLOCK_SIZE + entry_offset + MAX_FILE_NAME_LEN, SEEK_SET);
  write(disk, &avail_block_ind, sizeof(avail_block_ind));
  write(disk, &new_file_size, sizeof(new_file_size));

  // Set FAT table entry
  disk_pos = lseek(disk, avail_block_ind * FAT_RECORD_BYTE_LEN, SEEK_SET);
  write(disk, &end_block_code, sizeof(end_block_code));

  return avail_block_ind;
}

int format_disk(const int disk) {
  int16_t file_table[4096]  = { [0 ... 4095 ] = FREE_BLOCK_CODE};
  int8_t empty_block[4096] = {0};

  file_table[0] = SPECIAL_BLOCK_CODE;
  file_table[1] = SPECIAL_BLOCK_CODE;
  file_table[2] = END_BLOCK_CODE;

  // Write File allocation Table
  lseek(disk, 0, 0);
  if (write(disk, &file_table, 4096 * 2) != 4096 * 2) {
    printf("Failed to write to file. Sad.\n");
    return -1;
  }

  // Write Empty blocks
  for (size_t i = 2; i < 4096; i++) {
    if (write(disk, &empty_block, 4096) != 4096) {
      printf("Failed to write to file. Sad.\n");
      return -1;
    }
  }

  return 0;
}

/**
 * Create and initialize file if unexisiting or inproperly formatted.
 * TODO: File will always exists because opened for file descriptor.
 */
int verify_disk(int disk, int inv_errs) {
  // Check length of file
  struct stat info_buf;
  fstat(disk, &info_buf);

  if (info_buf.st_size != DISK_SIZE && !inv_errs) {
    printf("Disk file not formatted correctly.\n");
    return -1;
  } else {
    printf("Disk File Exists.\n");
    return -1;
  }

  // TODO: Implement File allocation table check

  // Passed all implemented checks
  return 0;
}
