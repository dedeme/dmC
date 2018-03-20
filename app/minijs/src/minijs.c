// Copyright 18-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "minijs.h"
#include "lexer/lexer.h"

static void help (void) {
  puts(
    "minijs. v201803\n\n"
    "Use:\n"
		"  minijs [OPTIONS] <main_file> \n"
		"  or\n"
		"  minijs -h\n\n"
    "OPTIONS:\n"
    "  -p <path>  List of source paths separated with '.'\n\n"
    "main_file:\n"
    "  mini file without extension\n\n"
    "Examples:\n"
    "  minijs MyProgram\n"
    "  minijs -p /home/my/lib1:lib2 MyProgram\n"
  );
}

int main (int argc, char **argv) {
  sys_init("minijs");

  char *main_file = "";
  Arr/*char*/ *paths = arr_new();
  arr_add(paths, "./");
  switch (argc) {
  case 2: {
    char *arg = argv[1];
    if (!strcmp(arg, "-h")) {
      help();
    } else {
      main_file = str_printf("%s.mini", arg);
    }
    break;
  }
  default:
    help();
  }

  if (*main_file) {
    char *root = lexer_run(paths, main_file);
    puts(root);
  }

  return 0;
}
