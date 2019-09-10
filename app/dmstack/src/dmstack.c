// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Process starter.

#include "dmstack.h"
#include "dmc/rnd.h"
#include "primitives.h"
#include "Reader.h"
#include "Machine.h"
#include "fails.h"
#include "DEFS.h"

int main (int argc, char *argv[]) {
  exc_init();
  rnd_init();
  primitives_init();
  fails_init();

  if (argc != 2) {
    EXC_RANGE(argc, 2, 2);
  };

  char *f = argv[1];
  if (!str_ends(f, ".dms")) f = str_cat(f, ".dms", NULL);
  if (!file_exists(f)) {
    EXC_IO(str_f("File '%s' not found"));
  }

  Reader *r = reader_new(f, file_read(f), 1, 0);
  machine_process(
    str_left(f, -4), list_cons(list_new(), machine_new_root()),
    reader_process(r)
  );
}
