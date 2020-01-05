// Functions related to the basic operations of the filesystem.

#include "disk.h"
#include <stdio.h>
#include <errno.h>

int dir_size(const int disk) {
  // Finds the size of the directory by iterating over its entries.

  int entry_cnt = 0;
  int16_t block_ind = DIR_START_BLOCK_INDEX;
  // Name of this variable is used by offsetof
  int16_t start;
  // TODO: Differentiate variable and offsetof variable
  //// Holds each directory entry's (FileRecord) start variable
  //int16_t file_start;
  off_t disk_pos;

  // While loop is included in case directory is more than 1 block.
  // This is not supported yet, TODO: Omit while loop
  while (block_ind != -2) {
    printf("Active block: %i\n", block_ind);

    disk_pos = lseek(disk, block_ind * BLOCK_SIZE + offsetof(struct FileRecord, start), SEEK_SET);
    read(disk, &start, sizeof(start));
    printf("First file start block: %i @ %li\n", start, disk_pos);

    // TODO: Support linked dir.
    // Loop over the entries in the directory to find the directory size.
    // Stops upon any special values (i.e. the directory's index or lower)
    // This will stop upon the 0's that fill the directory's empty entries.
    while (start > DIR_START_BLOCK_INDEX /* && entry_cnt % 4096 != 0 */) {
      entry_cnt++;

      // Seek to beginning of next entry's start field.
      disk_pos = lseek(disk, DIR_RECORD_BYTE_LEN - sizeof(start), SEEK_CUR);
      read(disk, &start, sizeof(start));
      printf("%i file start block: %i @ %li\n", entry_cnt + 1, start, disk_pos);
    }

    // TODO: Omit b/c no linked directory support
    lseek(disk, block_ind * FAT_RECORD_BYTE_LEN, SEEK_SET);
    read(disk, &block_ind, sizeof(start));
  }

  printf("Entry Count: %i\n", entry_cnt);
  // Return directory size in bytes
  return entry_cnt * DIR_RECORD_BYTE_LEN;
}

int16_t first_avail_block(const int disk) {
  int16_t fat_entry;
  lseek(disk, 0, SEEK_SET);

  // TODO: This be divided by the entry size?
  // FAT is two blocks long
  for (int i = 0; i < BLOCK_SIZE * 2; i++) {
    read(disk, &fat_entry, sizeof(fat_entry));
    if (fat_entry == FREE_BLOCK_CODE)
      // I is the FAT index of the open block
      return i;
  }

  printf("Failed to write: disk full!");
  return -1;
}

// NOTE: For future support of linked-directory feature.
//int extend_dir(const int disk) {
//  return 0;
//}

int read_file(const int disk, struct FileRecord *file) {
  int16_t block_index = file->start;
  char block[BLOCK_SIZE];

  // Iterate over blocks of file
  while (block_index != END_BLOCK_CODE) {

    lseek(disk, block_index * BLOCK_SIZE, SEEK_SET);
    read(disk, &block, BLOCK_SIZE);
    // Write block content to stdout
    write(STDOUT_FILENO, &block, BLOCK_SIZE);

    // Move to next block
    lseek(disk, block_index * FAT_RECORD_BYTE_LEN, SEEK_SET);
    read(disk, &block_index, sizeof(block_index));
  }

  return 0;
}

int get_file_and_start_block(const int disk, const char **write_name,
                               struct FileRecord *file) {
  if (lseek(disk, DIR_START_BLOCK_INDEX * 4096, SEEK_SET) < 0) {
    perror("Lseek failed in get_file_and_start_block");
    return -1;
  }

  // TODO Support linked dir
  // Iterates over directory entries to find matching file
  for (int i = 0; i < BLOCK_SIZE / DIR_RECORD_BYTE_LEN; i++) {
    // Read each name into the file struct
    read(disk, &(file->file_name), MAX_FILE_NAME_LEN);

    // If name matches, fills the rest of the FileRecord
    if (!strcmp((const char*)&(file->file_name), *write_name)) {
      // After reading the file name the 'disk head' is at the start field
      read(disk, &(file->start), sizeof(file->start) + sizeof(file->size));
      file->dir_loc = i * DIR_RECORD_BYTE_LEN;
      return 0;
    }

    if (lseek(disk, DIR_RECORD_BYTE_LEN - MAX_FILE_NAME_LEN, SEEK_CUR) < 0) {
      perror("Lseek failed in get_file_and_start_block");
      return -1;
    }
  }
  // No matching file
  // TODO: Consider named constant
  return -2;
}

int append_poem(const int disk, struct FileRecord *file, char *buffer, int32_t *buff_size) {
  off_t disk_pos;
  int16_t block_index;
  int16_t next_block_index = file->start;
  // If file exists, use this offset to not overwrite.
  int16_t block_offset = file->size % BLOCK_SIZE;


  // Write file size to dir
  disk_pos = lseek(disk,
                   DIR_START_BLOCK_INDEX * BLOCK_SIZE
                   + file->dir_loc
                   + sizeof(file->file_name)
                   + sizeof(file->start),
                   SEEK_SET);
  write(disk, buff_size, sizeof(*buff_size));

  // Follow blocks until at last block
  while (next_block_index != END_BLOCK_CODE) {
    block_index = next_block_index;
    lseek(disk, block_index * FAT_RECORD_BYTE_LEN, SEEK_SET);
    read(disk, &next_block_index, sizeof(block_index));
  }


  while (strlen(buffer)) {
    // Seek to block
    disk_pos = lseek(disk, block_index * BLOCK_SIZE, SEEK_SET);

    // Write rest of buffer or 4096 bytes and move buffer pointer
    // TODO Support appending in middle of block
    int write_len = (strlen(buffer) < BLOCK_SIZE ? strlen(buffer) : BLOCK_SIZE)
      - block_offset;

    write(disk, buffer, write_len);
    buffer += write_len;
    // After writing until the end of a block, the offset is zero.
    block_offset = 0;

    // If more bytes in buffer: allocate new block
    if (strlen(buffer)) {
      block_index = assign_block(disk, block_index);
    }
  }

  mark_end_block(disk, block_index);

  return 0;
}

int mark_end_block(const int disk, int16_t file_index) {
  // Variable is needed for sizeof() operation
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
  // Variables are needed for sizeof() operation
  const int16_t end_block_code = END_BLOCK_CODE;
  const int16_t free_block_code = FREE_BLOCK_CODE;
  off_t disk_pos;

  // Clear file size.
  disk_pos = lseek(disk,
                   DIR_START_BLOCK_INDEX * BLOCK_SIZE
                   + file_index * DIR_RECORD_BYTE_LEN
                   + MAX_FILE_NAME_LEN
                   + sizeof(int16_t),
                   SEEK_SET);
  write(disk, &empty_block, sizeof(int32_t));

  // Clear First block
  disk_pos = lseek(disk, file_index * BLOCK_SIZE, SEEK_SET);
  write(disk, &empty_block, BLOCK_SIZE);

  // Assign the first block as an end block in FAT table
  disk_pos = lseek(disk, file_index * FAT_RECORD_BYTE_LEN, SEEK_SET);
  // Read next file index
  read(disk, &file_index, sizeof(file_index));
  // Seek back to same FAT entry
  disk_pos = lseek(disk, sizeof(file_index) * -1, SEEK_CUR);
  write(disk, &end_block_code, sizeof(end_block_code));

  while (file_index != FREE_BLOCK_CODE) {
    // Free allocation in FAT table
    disk_pos = lseek(disk, file_index * FAT_RECORD_BYTE_LEN, SEEK_SET);
    write(disk, &free_block_code, sizeof(free_block_code));

    // Assign as end block in FAT table
    disk_pos = lseek(disk, file_index * FAT_RECORD_BYTE_LEN, SEEK_SET);
    // Read next file index
    read(disk, &file_index, sizeof(file_index));
    // Seek back to same FAT entry
    disk_pos = lseek(disk, sizeof(file_index) * -1, SEEK_CUR);
    write(disk, &end_block_code, sizeof(end_block_code));

  }

  return 0;
}


int create_file(const int disk, const char **file_name, struct FileRecord *file) {
  // Variable is needed for sizeof() operation
  int16_t end_block_code = END_BLOCK_CODE;
  off_t disk_pos;

  // Fill out FileRecord struct
  int name_len = (strlen(*file_name) < MAX_FILE_NAME_LEN) ?
                  strlen(*file_name) : MAX_FILE_NAME_LEN;
  strncpy(file->file_name, *file_name, name_len);

  file->start = first_avail_block(disk);
  if (file->start <= DIR_START_BLOCK_INDEX)
    return -1;

  file->size = 0;

  file->dir_loc = dir_size(disk);
  printf("Creating file at Dir index: %i\n", file->dir_loc);

  // TODO Support linked dir.
  // Check size for expansion
  /* if (dir_size(disk) / LOCK_SIZE >= 1) { */
  /*   extend_dir(disk); */
  /* } */

  // Find first available dir entry
  disk_pos = lseek(disk, DIR_START_BLOCK_INDEX * BLOCK_SIZE + file->dir_loc, SEEK_SET);

  // Write file_name, assign first block, size=0
  write(disk, *file_name, name_len);
  disk_pos = lseek(disk, DIR_START_BLOCK_INDEX * BLOCK_SIZE
                   + file->dir_loc + MAX_FILE_NAME_LEN, SEEK_SET);
  write(disk, &file->start, sizeof(file->start));
  write(disk, &file->size, sizeof(file->size));

  // Set FAT table entry
  disk_pos = lseek(disk, file->start * FAT_RECORD_BYTE_LEN, SEEK_SET);
  write(disk, &end_block_code, sizeof(end_block_code));


  return 0;
}

int format_disk(const int disk) {
  int16_t file_table[4096]  = { [0 ... 4095 ] = FREE_BLOCK_CODE};
  int8_t empty_block[4096] = {0};

  // Establish FAT and Directoy blocks
  file_table[0] = SPECIAL_BLOCK_CODE;
  file_table[1] = SPECIAL_BLOCK_CODE;
  // Directory noted with an end block code to allow for a linked directory
  // structure in future.
  file_table[2] = END_BLOCK_CODE;

  // Write File Allocation Table
  lseek(disk, 0, 0);
  if (write(disk, &file_table, 4096 * 2) != 4096 * 2) {
    printf("Failed to write to file.\n");
    return -1;
  }

  // Write Empty blocks (including empty directory)
  for (size_t i = 2; i < 4096; i++) {
    if (write(disk, &empty_block, 4096) != 4096) {
      printf("Failed to write to file.\n");
      return -1;
    }
  }

  return 0;
}

void read_fat_table(int disk, int16_t *table) {
  lseek(disk, 0, SEEK_SET);
  read(disk, table, 2 * BLOCK_SIZE);
}
