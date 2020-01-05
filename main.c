#include <unistd.h>
#include <fcntl.h>

#include "argtable3.h"
#include "disk.h"

extern int get_poem_len(const char **poem_name);
extern void get_poem(const char **poem_name, char* poem_buffer);

const char* DEF_LOC = "disk.isoish";

enum subcmd {READ, WRITE, APPEND, FORMAT, MAP};


// Reads file by file_name and prints to stdout
// Args:
//   disk: File descriptor of the 'disk'
// Returns:
//    0 on success
//   -1 on failure
int read_file_cmd(const int disk, const char **file_name) {
  // Find file by file_name
  struct FileRecord file;
  int success = get_file_and_start_block(disk, file_name, &file);
  if (success == -1) return -1;

  if (success == -2) {
    printf("File not found.");
    return -1;
  }

  // Read to stdout
  if (read_file(disk, &file) == 0) return -1;

  return 0;
}

// Creates a file and fills with poem named `poem_name`
// Args:
//   disk: File descriptor of the 'disk'
//   file_name: Pointer to the file name to be searched for
//   poem_name: Name of the poem in the poety_map file to write into the file.
// Returns:
//    0 on success
//   -1 on failure
int file_write(const int disk, const char **file_name, const char **poem_name) {
  struct FileRecord file;

  // Search for matching file record
  int success = get_file_and_start_block(disk, file_name, &file);
  if (success == -1)
    return -1;

  // If no matching file name, create; Otherwise, clear for overwrite.
  if (success == -2) {
    if (create_file(disk, file_name, &file) < 0) {
      printf("Failed to Create file.");
      return -1;
    }
  } else {
    clear_file(disk, file.start);
  }

  // Check that poem exists
  // TODO Optimize for file writes
  int32_t buff_size = get_poem_len(poem_name);
  // TODO This leaves a half-made file
  if (buff_size == 0) {
    printf("Invalid Poem name.");
    return -1;
  }

  // Write poem to file
  char* buffer = malloc(buff_size);
  get_poem(poem_name, buffer);
  printf("Length of poem \"%s\": %i\n", *poem_name, buff_size);
  append_poem(disk, &file, buffer, &buff_size);

  free(buffer);

  // Assumes success
  return 0;
}

// Appends a poem named `poem_name` to an existing file.
// Args:
//   disk: File descriptor of the 'disk'
//   file_name: Pointer to the file name to be searched for
//   poem_name: Name of the poem in the poety_map file to append to the file.
// Returns:
//    0 on success
//   -1 on failure
int file_append(const int disk, const char **file_name, const char **poem_name) {
  //TODO: Consider names constants for error codes
  struct FileRecord file;

  // Search for matching file record
  int success = get_file_and_start_block(disk, file_name, &file);
  if (success == -1)
    return -1;

  // If no matching file name, create file
  if (success == -2) {
    if (create_file(disk, file_name, &file) < 0) {
      printf("Failed to Create file.");
      return -1;
    }
  }

  // TODO Optimize for file writes
  int32_t buff_size = get_poem_len(poem_name);
  // TODO This leaves a half-made file.
  if (buff_size == 0) {
    printf("Invalid Poem name.");
    return -1;
  }

  // Write poem to file
  char* buffer = malloc(buff_size);
  get_poem(poem_name, buffer);
  printf("Length of poem \"%s\": %i\n", *poem_name, buff_size);
  append_poem(disk, &file, buffer, &buff_size);

  free(buffer);

  // Assume success
  return 0;
}

// Erase and setup disk file.
// Args:
//   disk: File descriptor of the 'disk'
int format(const int disk) {
  return format_disk(disk);
}

// Prints a visual representation of the disk file to stdout.
//   F - FAT table
//   D - Directory
//   * - Occupied block
//   _ - Free block
// Args:
//   disk: File descriptor of the 'disk'
// Returns: 0 (asssumes success)
int map(const int disk) {
  int16_t table[BLOCK_SIZE];
  char map[BLOCK_SIZE];
  read_fat_table(disk, (int16_t*)&table);

  // Assumes the FAT and Directory are in first 3 blocks
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

// Opens the file and executes the corresponding command function.
// Args:
//   sc: Subcommand to execute with file open
//   file_name: Name of file for subcommand to use
//   poem_name: Name of poem for subcommand to use
// Returns:
//    0 on success
//   -1 on failure
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

// Sets-up and uses Argparse to choose correct function.
int main(int argc, char **argv) {
  const char* prog_name = "phile";

  /// Define all subcommand syntaxes.
  // Each element of a syntax (e.g. regex match, string) is constructed by
  // Argparse into a struct and then combined in an array. The array is passed
  // to the arg_parse function to define the syntax.

  // Read
  struct arg_rex *sbcmd_rd = arg_rex1(NULL, NULL, "read", NULL, ARG_REX_ICASE,
                                      "Read and display contents of a file");
  struct arg_str *file_str_rd = arg_str1(NULL, NULL, "<filename>", "File name");
  struct arg_end *end_rd = arg_end(20);
  void* argtable_rd[] = {sbcmd_rd, file_str_rd, end_rd};

  // Write
  struct arg_rex *sbcmd_wr = arg_rex1(NULL, NULL, "write", NULL, ARG_REX_ICASE,
                                      "Write file to the disk");
  struct arg_str *file_str_wr = arg_str1(NULL, NULL, "<filename>", "File name");
  struct arg_str *poem_str_wr = arg_str1(NULL, NULL, "<poemname>", "Poem name");
  struct arg_end *end_wr = arg_end(20);
  void* argtable_wr[] = {sbcmd_wr, file_str_wr, poem_str_wr, end_wr};

  // Append
  struct arg_rex *sbcmd_ap = arg_rex1(NULL, NULL, "append", NULL, ARG_REX_ICASE,
                                      "Append file to the disk");
  struct arg_str *file_str_ap = arg_str1(NULL, NULL, "<filename>", "File name");
  struct arg_str *poem_str_ap = arg_str1(NULL, NULL, "<poemname>", "Poem name");
  struct arg_end *end_ap = arg_end(20);
  void* argtable_ap[] = {sbcmd_ap, file_str_ap, poem_str_ap, end_ap};

  // Format
  struct arg_rex *sbcmd_fmt = arg_rex1(NULL, NULL, "format", NULL,
                                       ARG_REX_ICASE, NULL);
  struct arg_end *end_fmt = arg_end(20);
  void* argtable_fmt[] = {sbcmd_fmt, end_fmt};

  // Map
  struct arg_rex *sbcmd_mp = arg_rex1(NULL, NULL, "map", NULL, ARG_REX_ICASE,
                                      NULL);
  struct arg_end *end_mp = arg_end(20);
  void* argtable_mp[] = {sbcmd_mp, end_mp};

  /// Parse!
  // arg_parse takes the arguments and a syntax from above and returns the
  // number of errors produced by parsing against that syntax.
  int nerrors_rd = arg_parse(argc, argv, argtable_rd);
  int nerrors_wr = arg_parse(argc, argv, argtable_wr);
  int nerrors_ap = arg_parse(argc, argv, argtable_ap);
  int nerrors_fmt = arg_parse(argc, argv, argtable_fmt);
  int nerrors_mp = arg_parse(argc, argv, argtable_mp);

  /// Execute function for given subcommand
  // By checking for a syntax that produced no errors, the correct subcommand is
  // found.
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
  // Only runs if no commands were run.
  // Checks if a subcommand was included and prints its errors if so.
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
    // TODO: Better usage: options and command help.
    printf("usage: %s <command> [<args>]\n", prog_name);
  }

  /// Free Memory
  // TODO: Make sure this is run when commands are as well. (proc exits anyways)
  arg_freetable(argtable_rd, sizeof(argtable_rd)/sizeof(argtable_rd[0]));
  arg_freetable(argtable_wr, sizeof(argtable_wr)/sizeof(argtable_wr[0]));
  arg_freetable(argtable_ap, sizeof(argtable_ap)/sizeof(argtable_ap[0]));
  arg_freetable(argtable_fmt, sizeof(argtable_fmt)/sizeof(argtable_fmt[0]));
  arg_freetable(argtable_mp, sizeof(argtable_mp)/sizeof(argtable_mp[0]));

  // TODO: Return proper exit code
  return 0;
}
