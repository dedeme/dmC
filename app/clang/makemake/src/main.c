// Copyright 24-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "main.h"
//#include "assert.h"
#include "reader.h"
#include "encoder.h"
#include "DEFS.h"

static void help (void) {
  puts(
    "Use in a root directory with 'bin', 'include' and 'src' subdirectories:\n"
    "  makemake [--help | --prg <name> [--libs <libs>]]\n"
    "For example:\n"
    "  makemake\n"
    "  makemake --help\n"
    "  makemake --prg myprogram\n"
    "  makemake --prg myprogram --libs dmc\n"
    "  makemake --prg myprogram --libs dmc:market"
  );
}

int main (int argc, char *argv[]) {
  sys_init("makemake");

  if (!reader_check_directories()) {
    puts("Fail checking directories");
    help();
    return 1;
  }

  if (argc == 2 && str_eq(argv[1], "--help")) {
    help();
    return 0;
  }

  char *prg = "";
  // Arr[char]
  Arr *libs = arr_new();
  if (argc == 3 || argc == 5) {
    if (str_eq(argv[1], "--prg")) {
      prg = argv[2];
      if (argc == 5) {
        if (str_eq(argv[3], "--libs")) {
          libs = str_csplit(argv[4], ':');
        } else {
          puts ("Expected '--prg'");
          help();
          return 1;
        }
      }
    } else {
      puts ("Expected '--prg'");
      help();
      return 1;
    }
  } else {
    puts("Wrong number of arguments");
    help();
    return 1;
  }

  if (!reader_check_libs(libs)) return 1;

  Node1 *n1 = opt_nget(reader_mk_tree());
  if (!n1) return 1;

  // puts(node1_to_str(n1));

  // Arr[Node2]
  Arr *ns = arr_new();
  Node2 *n2 = node1_to_node2(ns, n1, 0);

  // puts(node2_to_str(n2));
  //assert(str_eq(node1_to_str(n1), node2_to_str(n2)));

  node2_sort(ns);
  node2_optimize(ns);

  file_del(MK_DIR);
  file_mkdir(MK_DIR);

  EACH(ns, Node2, n)
    if (node2_depth(n)) {
      char *mkf = str_f("%s/%s/%s/Makefile", MK_DIR, LIB_DIR, node2_path(n));
      file_mkdir(path_parent(mkf));
      file_write(mkf, encoder_makefile(libs, n));
      file_mkdir(str_f("%s/%s/%s", MK_DIR, OBJ_DIR, node2_path(n)));
    }
  _EACH

  file_write("Makefile", encoder_main_makefile(prg, libs, ns, n2));
  file_write(MK_FILE, encoder_mk(ns));

  return 0;
}
