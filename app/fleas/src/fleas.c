// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "fleas.h"
#include "models.h"
#include "io.h"
#include "Model.h"
#include "models/MMBack1.h"
#include "process.h"
#include "util.h"
#include "DEFS.h"

static void help() {
  puts(
    "fleas. v201902\n\n"
    "Usage:\n"
    "  fleas\n"
    "    Calculate values of the best flea.\n"
    "    (e.g. fleas)\n"
    "  fleas all\n"
    "    Calculate values of every flea\n"
    "    (e.g. fleas all)\n"
    "  fleas help\n"
    "    Show this message\n"
    "    (e.g. fleas help)\n"
  );
}

int main(int argc, char **args) {
  sys_init("fleas");

  if (argc == 2 && str_eq(args[1], "all")) {
    // Arr[Model]
    Arr *models = models_create_new();

    io_init(models);

    EACH(models, Model, m)
      util_print_model_name(m);

      do {} while(process_run(m));
    _EACH

    arr_free(models);
    io_free();
  } else if (argc == 2 && str_eq(args[1], "help")) {
    help();
  } else if (argc > 1) {
    help();
  } else {
    // Arr[Model]
    Arr *models = arr_new((FPROC)model_free);
    Model *m = mMBack1_new();
    arr_push(models, m);

    io_init(models);

    util_print_model_name(m);
    do {} while(process_run(m));

    arr_free(models);
    io_free();
  }

  sys_end();
  return 0;
}
