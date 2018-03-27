// Copyright 18-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "minijs.h"
#include "Cpath.h"
#include "lexer/lexer.h"
#include "parser/Program.h"
#include "DEFS.h"

static void help (void) {
  printf("minijs. v%s\n\n", VERSION);
  puts(
    "Use:\n"
		"  minijs [OPTIONS] <main_file> \n"
		"  or\n"
		"  minijs -h\n\n"
    "OPTIONS:\n"
    "  -js <path> File '.js'.\n"
    "      Default: index.js in the same directory that <main_file>\n"
    "  -p <paths> List of source paths separated with ':'\n"
    "      Automaticaly added current directory.\n\n"
    "main_file:\n"
    "  mini file without extension\n\n"
    "Examples:\n"
    "  minijs MyProgram\n"
    "  minijs -p /home/my/lib1:lib2 MyProgram\n"
  );
}

int main (int argc, char **argv) {
  sys_init("minijs");

  char *js = "";
  Arr/*char*/ *paths = arr_new();
  char *main_file = "";

  bool js_set = false;
  bool p_set = false;

  if (argc == 1) {
    help();
    return 0;
  }

  int c = 1;
  while(c < argc) {
    char *arg = argv[c];
    if (!strcmp(arg, "-js")) {
      if (js_set) {
        puts("Duplicate option -js");
        help();
        return 0;
      }
      js_set = true;
      ++c;
      if (c >= argc) {
        puts("Argument for -js is missing");
        help();
        return 0;
      }
      js = argv[c++];
    } else if (!strcmp(arg, "-p")) {
      if (p_set) {
        puts("Duplicate option -p");
        help();
        return 0;
      }
      p_set = true;
      ++c;
      if (c >= argc) {
        puts("Argument for -p is missing");
        help();
        return 0;
      }
      paths = str_csplit(argv[c++], ':');
    } else if (!strcmp(arg, "-h")) {
      ++c;
      if (c < argc || c != 2) {
        puts("-h must be the only argument");
        help();
        return 0;
      }
      help();
      return 0;
    } else {
      ++c;
      if (c < argc) {
        puts("<main_file> must be the last argument");
        help();
        return 0;
      }
      main_file = arg;
    }
  }

  if (!main_file) {
    puts("<main_file is missing>");
    return 0;
  }

  if (!it_contains_str(it_from(paths), ".")) {
    arr_insert(paths, 0, ".");
  }

  cpath_init(paths);
  Cpath *main_path = cpath_new(main_file);

  if (!*js) {
    js = path_cat(path_parent(cpath_file(main_path)), "index.js", NULL);
  }
  file_mkdir(path_parent(js));

  lexer_run(paths, main_path);
  program_print(program_new(main_path), js);

  return 0;
}
