// Copyright 26-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "fleas.h"
#include "io.h"
#include "process.h"
#include "bprocess.h"

static void help(void) {
  puts(
    "fleas. v201810\n"
    "\n"
    "Usage:\n"
    "  fleas\n"
    "    Calculate fleas values\n"
    "    (e.g. fleas)\n"
    "  fleas clean\n"
    "    Delete all data and set cycle to 0\n"
    "    (e.g. fleas clean)\n"
    "  fleas stop\n"
    "    Send a stop signal when fleas was called with 'fleas t'\n"
    "    (e.g. fleas stop)\n"
    "  fleas unlock\n"
    "    Remove the lock used to control double runs\n"
    "    (e.g. fleas unlock)\n"
    "  fleas help\n"
    "    Show this message\n"
    "    (e.g. fleas help)\n"
  );
}

int main(int argc, char *arg[]) {
  sys_init("fleas");

  if (argc == 2 && str_eq(arg[1], "unlock")) {
    io_unlock();
    sys_end();
    return 0;
  }

  if (argc == 2 && str_eq(arg[1], "stop")) {
    io_send_stop();
    sys_end();
    return 0;
  }
  if (io_lock()) {
    sys_end();
    return 0;
  }

  if (argc == 2 && str_eq(arg[1], "clean")) {
    io_clean();
    sys_end();
    return 0;
  }

  if (argc == 2 && str_eq(arg[1], "help")) {
    help();
    io_unlock();
    sys_end();
    return 0;
  }

  io_init();

  process_run();
  bprocess_run();

  io_unlock();
  io_end();
  sys_end();
}
