// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "fleas.h"
#include "models.h"
#include "io.h"
#include "Model.h"
#include "process.h"
#include "util.h"
#include "DEFS.h"

static void help() {
  puts(
    "fleas2. v201902\n\n"
    "Usage:\n"
    "  fleas2\n"
    "    Calculate fleas2 values\n"
    "    (e.g. fleas2)\n"
    "  fleas2 clean\n"
    "    Delete all data and set cycle to 0\n"
    "    (e.g. fleas2 clean)\n"
    "  fleas2 stop\n"
    "    Send a stop signal.\n"
    "    (e.g. fleas2 stop)\n"
    "  fleas2 unlock\n"
    "    Remove the lock used to control double runings\n"
    "    (e.g. fleas2 unlock)\n"
    "  fleas2 help\n"
    "    Show this message\n"
  );
}

int main(int argc, char **args) {
  sys_init("fleas2");

  if (argc > 1) {
    help();
  } else {
    // Arr[Model]
    Arr *models = models_create_new();

    io_init(models);

    EACH(models, Model, m)
      util_print_model_name(m);

      process_run(m);
    _EACH

    arr_free(models);
    io_free();
  }

  sys_end();
  return 0;
}
