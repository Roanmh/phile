#include "argtable3.h"

int read(const char **file_name) {
  printf("I read %s! I read %s!\n", *file_name, *file_name);
  return 0;
}

int format() {
  printf("\"format format\" ... \"format format\"\n");
  return 0;
}

int write(const char **file_name, const char **poem_name) {
  printf("%s: %s\n", *file_name, *poem_name);
  return 0;
}

int append(const char **file_name, const char **poem_name) {
  printf("Append %s: %s\n", *file_name, *poem_name);
  return 0;
}

int map() {
  printf("I'm the map! I'm the map!\n");
  return 0;
}

int main(int argc, char **argv) {
  const char* prog_name = "phile";
  /// Define all subcommand syntaxes.
  // Read
  struct arg_rex *sbcmd_rd = arg_rex1(NULL, NULL, "read", NULL, ARG_REX_ICASE, "Read and display contents of a file");
  struct arg_str *file_str_rd = arg_str1(NULL, NULL, "<filename>", "File name");
  struct arg_end *end_rd = arg_end(20);
  void* argtable_rd[] = {sbcmd_rd, file_str_rd, end_rd};
  int nerrors_rd;

  // Write
  struct arg_rex *sbcmd_wr = arg_rex1(NULL, NULL, "write", NULL, ARG_REX_ICASE, "Write file to the disk");
  struct arg_str *file_str_wr = arg_str1(NULL, NULL, "<filename>", "File name");
  struct arg_str *poem_str_wr = arg_str1(NULL, NULL, "<poemname>", "Poem name");
  struct arg_end *end_wr = arg_end(20);
  void* argtable_wr[] = {sbcmd_wr, file_str_wr, poem_str_wr, end_wr};
  int nerrors_wr;

  // Append
  struct arg_rex *sbcmd_ap = arg_rex1(NULL, NULL, "append", NULL, ARG_REX_ICASE, "Append file to the disk");
  struct arg_str *file_str_ap = arg_str1(NULL, NULL, "<filename>", "File name");
  struct arg_str *poem_str_ap = arg_str1(NULL, NULL, "<poemname>", "Poem name");
  struct arg_end *end_ap = arg_end(20);
  void* argtable_ap[] = {sbcmd_ap, file_str_ap, poem_str_ap, end_ap};
  int nerrors_ap;

  // Format
  struct arg_rex *sbcmd_fmt = arg_rex1(NULL, NULL, "format", NULL, ARG_REX_ICASE, NULL);
  struct arg_end *end_fmt = arg_end(20);
  void* argtable_fmt[] = {sbcmd_fmt, end_fmt};
  int nerrors_fmt;

  // Map
  struct arg_rex *sbcmd_mp = arg_rex1(NULL, NULL, "map", NULL, ARG_REX_ICASE, NULL);
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
    return read(file_str_rd->sval);
  else if (nerrors_wr == 0)
    return write(file_str_wr->sval, poem_str_wr->sval);
  else if (nerrors_ap == 0)
    return append(file_str_ap->sval, poem_str_ap->sval);
  else if (nerrors_fmt == 0)
    return format();
  else if (nerrors_mp == 0)
    return map();

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
