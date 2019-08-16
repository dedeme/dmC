// Copyright 24-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "makemake.h"
#include "DEFS.h"
#include "reader.h"
#include "writer.h"

static void help (void) {
  puts(
    "Use in a root directory with 'bin', 'include' and 'src' subdirectories:\n"
    "  makemake [--help | --prg <name> --main <file_name> [--libs <libs>]]\n"
    "For example:\n"
    "  makemake\n"
    "  makemake --help\n"
    "  makemake --prg myprogram --main main\n"
    "  makemake --prg myprogram --main main --libs dmc\n"
    "  makemake --prg myprogram --main main --libs dmc:market"
  );
}

// libs is Arr[char], os is Arr[char]
static void add_dependency (Arr *libs, Arr *os, Buf *bf, char *d) {
  int fn (char *f) { return str_eq(f, d); }
  if (it_contains(arr_to_it(os), (FPRED)fn)) {
    return;
  }

  arr_push(os, d);

  // Arr[char]
  Arr *includes = arr_new();
  EACH(libs, char, lib)
    arr_push(includes, str_f(" \\\n\t\t-I$(LIB_INCLUDE_%s)", lib));
  _EACH

  // Arr[char]
  Arr *deps = reader_dependencies(d);

  // Arr[char]
  Arr *hdeps = arr_new();
  EACH(deps, char, d)
    arr_push(hdeps, str_f("include/%s.h src/%s.c", d, d));
  _EACH

  buf_add(bf, str_f("%s/%s.o : ", OBJ_DIR, d));
  buf_add(bf, str_f("%s\n", str_cjoin(hdeps, ' ')));

  buf_add(bf, "\tgcc $(CFLAGS) ");
  buf_add(bf, str_f("-c src/%s.c ", d));
  buf_add(bf, str_f("-o %s/%s.o \\\n", OBJ_DIR, d));

  buf_add(bf, "\t\t-Iinclude");
  buf_add(bf, str_join(includes, ""));

  buf_add(bf, "\n\n");

  EACH(deps, char, d)
    add_dependency(libs, os, bf, d);
  _EACH
}

int main (int argc, char *argv[]) {
  if (reader_check_directories()) {
    help();
    return 1;
  }

  if (argc == 2 && str_eq(argv[1], "--help")) {
    help();
    return 0;
  }

  char *prg = "";
  char *src = "";
  // Arr[char]
  Arr *libs = arr_new();
  int c = 1;
  if (argc == 1 || argc == 3 || argc == 5 || argc == 7) {
    while (c < argc) {
      char *op = argv[c++];
      if (str_eq(op, "--prg")) {
        if (*prg) {
          puts("Option --prg duplicated");
          help();
          return 1;
        } else {
          prg = argv[c++];
        }
      } else if (str_eq(op, "--main")) {
        if (*src) {
          puts("Option --main duplicated");
          help();
          return 1;
        } else {
          src = argv[c++];
        }
      } else if (str_eq(op, "--libs")) {
        if (arr_size(libs)) {
          puts("Option --libs duplicated");
          help();
          return 1;
        } else {
          libs = str_csplit(argv[c++], ':');
        }
      } else {
        printf("Unkown option '%s'\n", op);
        help();
        return 1;
      }
    }
  } else {
    puts("Wrong number of arguments");
    help();
    return 1;
  }

  if (!*prg) prg = path_name(file_cwd());
  if (!*prg) {
    puts ("'Progam name' argument is missing");
    return 1;
  }

  if (!*src) src = path_name(file_cwd());
  if (!*src) {
    puts ("'Main file' argument is missing");
    return 1;
  }

  if (src[0] == '/') {
    printf("'%s' is an absolute path\n", src);
    help();
    return 1;
  }

  char *cfile = str_f("src/%s.c", src);
  if (!file_exists(cfile)) {
    printf("'%s' is missing\n", cfile);
    help();
    return 1;
  }

  if (reader_check_libraries(libs)) {
    help();
    return 1;
  }

  int mk_all = 0;

  if (writer_prune()) {
    mk_all = 1;
  } else if (!file_exists("Makefile")) {
    mk_all = 1;
  } else {
    // Opt[char]
    char *new_makefile = opt_nget(reader_check_changes());
    if (new_makefile) {
      if (*new_makefile) {
        file_write("Makefile", new_makefile);
      }
    } else {
      mk_all = 1;
    }
  }

  if (mk_all) {
    Buf *bf = buf_new();
    // Arr[char] os
    Arr *os = arr_new();

    add_dependency(libs, os, bf, src);

    writer_mkmake (prg, libs, src, os, buf_to_str(bf));
  }

  return 0;
}
