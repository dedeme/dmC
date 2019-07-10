// Copyright 09-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "main.h"
#include "help.h"
#include "book.h"
#include "update.h"


int main (int argc, char *argv[]) {
  exc_init();

  if (argc > 1) {
    if (argc == 3 && str_eq(argv[1], "book")) {
      book(argv[2]);
      return 0;
    }
    if (argc == 2 && str_eq(argv[1], "update")) {
      update();
      return 0;
    }
  }

  help();
  return 1;
}
