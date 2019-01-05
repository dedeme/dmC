// Copyright 21-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ctpl.h"
#include "process.h"

static void help(void) {
  puts(
    "Usage:\n"
    "  ctpl [<PATH> | -h | --help]\n"
    "'ctpl' process code below the directory 'PATH'.\n"
    "'PATH' must have two subdirectories: 'include' and 'src'.\n"
    "Options:\n"
    "  -h, --help: Show this message\n"
    "Examples:\n"
    "  ctpl ./\n"
    "  ctpl some/path\n"
    "  ctpl -h\n"
  );
}

int main(int argc, char *argv[]) {
  sys_init("ctpl");

  if (argc != 2) {
    help();
  } else {
    if (str_eq(argv[1], "-h") || str_eq(argv[1], "--help")) {
      help();
    } else {
      process_root(argv[1]);
    }
  }

  sys_end();
}
