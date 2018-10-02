// Copyright 28-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "fleas.h"
#include "io.h"
#include "cycle.h"
#include "time.h"

static void help (void) {
  puts(
    "fleas. v201809\n\n"
    "Usage:\n"
		"  fleas \n"
    "    Calulates only one cycle\n"
    "    (e.g. fleas)\n"
		"  fleas t <time_minutes>\n"
    "    Cycling <time_minutes>\n"
    "    (e.g. fleas t 3)\n"
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

int main (int argc, char **argv) {
  sys_init("fleas");
  io_init();

  if (argc == 2 && str_eq(argv[1], "unlock")) {
    io_unlock();
    return 0;
  }
  if (argc == 2 && str_eq(argv[1], "stop")) {
    io_send_stop();
    return 0;
  }
  if (!io_lock()){
    return 0;
  };
  io_remove_stop();

  if (argc == 2) {
    if (str_eq(argv[1], "clean")) {
      io_clean();
    } else {
      help();
    }
  } else if (argc == 3) {
    if (str_eq(argv[1], "t")) {
      int arg2 = atoi(argv[2]);
      if (arg2 < 1) {
        help();
      } else {
        int dead_line = arg2 * 60;
        time_t t = time(NULL);
        while (true) {
          cycle_run();
          if (io_stoped()) {
            break;
          }
          int df = (int)difftime(time(NULL), t);
          printf("Elapsed %d of %d minutes\n", df / 60, arg2);
          if (df > dead_line){
            break;
          }
        }
      }
    } else {
      help();
    }
  } else {
    cycle_run();
  }

  io_unlock();
  return 0;
}
