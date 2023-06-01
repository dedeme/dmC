// Copyright 27-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "main.h"
#include <signal.h>
#include <stdio.h>
#include "kut/DEFS.h"
#include "kut/str.h"
#include "kut/sys.h"
#include "kut/js.h"
#include "db.h"
#include "server.h"
#include "pgs.h"

static char *help =
  "Usage: "
  "\n     cmarket start"
  "\n       Starts server."
  "\n     cmarket stop"
  "\n       Stops server."
  "\n     cmarket init"
  "\n       Initialize database."
  "\n     cmarket <commandKey> <JSON>"
  "\n       Executes a JSON command."
;

static void shandler (int sig) {
  EXC_GENERIC("Arithmetic exception");
}

int main (int argc, char *argv[]) {
  if (argc < 2) {
    puts(help);
    return 0;
  }

  signal (SIGFPE, shandler);
  sys_init();

  char *param = argv[1];
  TRY {

    if (str_eq(param, "stop") && argc == 2) server_stop();
    else if (str_eq(param, "start") && argc == 2) server_start();
    else if (str_eq(param, "init") && argc == 2) db_init();
    else if (argc != 3) puts(help);
    else pgs_process(param, js_ro(argv[2]));

  } CATCH (e) {
      //--
      char *map (char *l) { return str_f("  %s", l); }
    puts(str_f(
      "%s\n%s", exc_msg(e), arr_join(arr_map(exc_stack(e), (FMAP)map), "\n")
    ));
  }_TRY
}
