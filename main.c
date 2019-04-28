#include <unistd.h>
#include <fcntl.h>

#include "argtable3.h"
#include "disk.h"

extern int get_poem_len(const char **poem_name);
extern void get_poem(const char **poem_name, char* poem_buffer);

const char* DEF_LOC = "disk.isoish";

enum subcmd {READ, WRITE, APPEND, FORMAT, MAP};

int read_file_cmd(const int disk, const char **file_name) {
  struct FileRecord file;
  int success = get_file_and_start_block(disk, file_name, &file);
  if (success == -1) return -1;

  if (success == -2) {
    printf("File not found.");
    return -1;
  }

  if (read_file(disk, &file) == 0) return -1;

  return 0;
}

int file_write(const int disk, const char **file_name, const char **poem_name) {
  struct FileRecord file;
  int success = get_file_and_start_block(disk, file_name, &file);
  if (success == -1) return -1;

  // If no matching file name
  if (success == -2) {
    if (create_file(disk, file_name, &file) < 0) {
      printf("Failed to Create file.");
      return -1;
    }
  } else {
    clear_file(disk, file.start);
  }

  // TODO Optimize for file writes
  int32_t buff_size = get_poem_len(poem_name);
  // TODO Make sure this doesn't leave a half-made file
  if (buff_size == 0) {
    printf("Invalid Poem name.");
    return -1;
  }
  char* buffer = malloc(buff_size);
  get_poem(poem_name, buffer);
  printf("Length of poem \"%s\": %i\n", *poem_name, buff_size);
  /* printf("%s\n", buffer); */
  append_poem(disk, &file, buffer, &buff_size);
  free(buffer);

  return 0;
}

int file_append(const int disk, const char **file_name, const char **poem_name) {
  struct FileRecord file;
  int success = get_file_and_start_block(disk, file_name, &file);
  if (success == -1) return -1;

  // If no matching file name
  if (success == -2) {
    if (create_file(disk, file_name, &file) < 0) {
      printf("Failed to Create file.");
      return -1;
    }
  } else {
    /* clear_file(disk, file.start); */ // Append by not deteling the sutfz
  }

  // TODO Optimize for file writes
  int32_t buff_size = get_poem_len(poem_name);
  // TODO Make sure this doesn't leave a half-made file (It does)
  if (buff_size == 0) {
    printf("Invalid Poem name.");
    return -1;
  }
  char* buffer = malloc(buff_size);
  get_poem(poem_name, buffer);
  printf("Length of poem \"%s\": %i\n", *poem_name, buff_size);
  /* printf("%s\n", buffer); */
  append_poem(disk, &file, buffer, &buff_size);
  free(buffer);

  return 0;
}

int format(const int disk) {
  return format_disk(disk);
}

int map(const int disk) {
  int16_t table[BLOCK_SIZE];
  char map[BLOCK_SIZE];
  read_fat_table(disk, (int16_t*)&table);

  map[0] = 'F';
  map[1] = 'F';
  map[2] = 'D';

  for (size_t i = 3; i < BLOCK_SIZE; i++) {
    switch (table[i]) {
    case FREE_BLOCK_CODE:
      map[i] = '_';
      break;
    default:
      map[i] = '*';
    }
  }

  for (size_t i = 0; i < BLOCK_SIZE; i++) {
    printf("%c", map[i]);
    if (!((i + 1) % 64)) printf("\n");
  }

  return 0;
}

int with_open_disk(enum subcmd sc, const char **file_name,
                   const char **poem_name) {
  const int disk = open(DEF_LOC, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  if (disk < 0) {
    printf("Disk access failed. Check for other programs or correct permissions");
    return -1;
  }

  switch (sc) {
  case READ:
    read_file_cmd(disk, file_name);
    break;
  case WRITE:
    file_write(disk, file_name, poem_name);
    break;
  case APPEND:
    file_append(disk, file_name, poem_name);
    break;
  case FORMAT:
    format(disk);
    break;
  case MAP:
    map(disk);
    break;
  }

  if (close(disk)) {
    printf("Disk access failed on close. This really shouldn't happen.");
    return -1;
  }
  return 0;
}

int main(int argc, char **argv) {
  const char* prog_name = "phile";
  /// Define all subcommand syntaxes.
  // Read
  struct arg_rex *sbcmd_rd = arg_rex1(NULL, NULL, "read", NULL, ARG_REX_ICASE,
                                      "Read and display contents of a file");
  struct arg_str *file_str_rd = arg_str1(NULL, NULL, "<filename>", "File name");
  struct arg_end *end_rd = arg_end(20);
  void* argtable_rd[] = {sbcmd_rd, file_str_rd, end_rd};
  int nerrors_rd;

  // Write
  struct arg_rex *sbcmd_wr = arg_rex1(NULL, NULL, "write", NULL, ARG_REX_ICASE,
                                      "Write file to the disk");
  struct arg_str *file_str_wr = arg_str1(NULL, NULL, "<filename>", "File name");
  struct arg_str *poem_str_wr = arg_str1(NULL, NULL, "<poemname>", "Poem name");
  struct arg_end *end_wr = arg_end(20);
  void* argtable_wr[] = {sbcmd_wr, file_str_wr, poem_str_wr, end_wr};
  int nerrors_wr;

  // Append
  struct arg_rex *sbcmd_ap = arg_rex1(NULL, NULL, "append", NULL, ARG_REX_ICASE,
                                      "Append file to the disk");
  struct arg_str *file_str_ap = arg_str1(NULL, NULL, "<filename>", "File name");
  struct arg_str *poem_str_ap = arg_str1(NULL, NULL, "<poemname>", "Poem name");
  struct arg_end *end_ap = arg_end(20);
  void* argtable_ap[] = {sbcmd_ap, file_str_ap, poem_str_ap, end_ap};
  int nerrors_ap;

  // Format
  struct arg_rex *sbcmd_fmt = arg_rex1(NULL, NULL, "format", NULL,
                                       ARG_REX_ICASE, NULL);
  struct arg_end *end_fmt = arg_end(20);
  void* argtable_fmt[] = {sbcmd_fmt, end_fmt};
  int nerrors_fmt;

  // Map
  struct arg_rex *sbcmd_mp = arg_rex1(NULL, NULL, "map", NULL, ARG_REX_ICASE,
                                      NULL);
  struct arg_end *end_mp = arg_end(20);
  void* argtable_mp[] = {sbcmd_mp, end_mp};
  int nerrors_mp;

  /// Parse!
  nerrors_rd = arg_parse(argc, argv, argtable_rd);
  nerrors_wr = arg_parse(argc, argv, argtable_wr);
  nerrors_ap = arg_parse(argc, argv, argtable_ap);
  nerrors_fmt = arg_parse(argc, argv, argtable_fmt);
  nerrors_mp = arg_parse(argc, argv, argtable_mp);

  /// Execute function for given subcommand
  if (nerrors_rd == 0)
    return with_open_disk(READ, file_str_rd->sval, NULL);
  else if (nerrors_wr == 0)
    return with_open_disk(WRITE, file_str_wr->sval, poem_str_wr->sval);
  else if (nerrors_ap == 0)
    return with_open_disk(APPEND, file_str_ap->sval, poem_str_ap->sval);
  else if (nerrors_fmt == 0)
    return with_open_disk(FORMAT, NULL, NULL);
  else if (nerrors_mp == 0)
    return with_open_disk(MAP, NULL, NULL);

  /// Return relevant errors
  if (sbcmd_rd->count > 0) {
    arg_print_errors(stdout, end_rd, prog_name);
    printf("usage: %s", prog_name);
    arg_print_syntax(stdout, argtable_rd, "\n");
  }
  else if(sbcmd_wr->count > 0) {
    arg_print_errors(stdout, end_wr, prog_name);
    printf("usage: %s", prog_name);
    arg_print_syntax(stdout, argtable_wr, "\n");
  }
  else if(sbcmd_ap->count > 0) {
    arg_print_errors(stdout, end_ap, prog_name);
    printf("usage: %s", prog_name);
    arg_print_syntax(stdout, argtable_ap, "\n");
  }
  else if (sbcmd_fmt->count > 0) {
    arg_print_errors(stdout, end_fmt, prog_name);
    printf("usage: %s", prog_name);
    arg_print_syntax(stdout, argtable_fmt, "\n");
  }
  else if(sbcmd_mp->count > 0) {
    arg_print_errors(stdout, end_mp, prog_name);
    printf("usage: %s", prog_name);
    arg_print_syntax(stdout, argtable_mp, "\n");
  }
  else {
    printf("usage: %s <command> [<args>]\n", prog_name);
  }

  /// Must free memory
  arg_freetable(argtable_rd, sizeof(argtable_rd)/sizeof(argtable_rd[0]));
  arg_freetable(argtable_wr, sizeof(argtable_wr)/sizeof(argtable_wr[0]));
  arg_freetable(argtable_ap, sizeof(argtable_ap)/sizeof(argtable_ap[0]));
  arg_freetable(argtable_fmt, sizeof(argtable_fmt)/sizeof(argtable_fmt[0]));
  arg_freetable(argtable_mp, sizeof(argtable_mp)/sizeof(argtable_mp[0]));

  return 0;
}
