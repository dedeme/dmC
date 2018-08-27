// Copyright 03-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <gc.h>
#include "dmc/sys.h"
#include "dmc/str.h"
#include "dmc/path.h"
#include "dmc/ct/Achar.h"
#include "dmc/ct/Ichar.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "ct/Classes.h"
#include "ct/Fclasses.h"
#include "io/main.h"
#include "io/io.h"
#include "io/Cpath.h"
#include "lexer/Tx.h"
#include "lexer/rclass.h"
#include "parser/pclass.h"
#include "writer/wjs.h"
#include "DEFS.h"

static void help(void) {
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

static void add_path(Achar *paths, char *path) {
  if (!ichar_contains(achar_to_it(paths), path)) {
    achar_add(paths, path);
  }
}

static void parse_main_header(Cpath *path) {
  THROW("") "Unimplemented" _THROW
}

int main_start (int argc, char **argv) {
  io_check_install();

  char *js = "index.js";
  Achar *paths = achar_new();
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
    if (str_eq(arg, "-js")) {
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
    } else if (str_eq(arg, "-p")) {
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
    } else if (str_eq(arg, "-h")) {
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

  if (!*main_file) {
    puts("<main_file is missing>");
    return 0;
  }

  add_path(paths, ".");
  add_path(paths, path_cat(sys_home(), "built", NULL));

  bool fail = false;
  TRY {
    cpath_init(paths);
    classes_init();
    fclasses_init();

    char *tmp = "";
    Cpath *main_path = cpath_new(tx_new(
      "Command Line", tmp, tmp, 0, 0),
      main_file
    );
    rclass(main_path);
    pclass(main_path);
    parse_main_header(main_path);
    wjs();
  } CATCH(e) {
    if (str_eq(exc_type(e), "tx")) {
      fail = true;
    } else {
      THROW(exc_type(e)) e _THROW
    }
  }_TRY

  puts(js);

  return fail ? 1 : 0;
}
