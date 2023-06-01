// Copyright 31-May-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "server.h"
#include <stdio.h>
#include "kut/sys.h"
#include "kut/DEFS.h"
#include "db.h"

void server_start (void) {
  for (;;) {
    sys_sleep(1000);
  }
}

void server_stop (void) {
  Rs *prgs_rs = sys_cmd("ps -x");
  if (!prgs_rs) {
    puts(rs_error(prgs_rs));
    return;
  }

  if (!db_lock())
    EXC_ILLEGAL_STATE("Date base locked when stopping");

  int ok = 0;
  EACH(str_csplit(rs_get(prgs_rs), '\n'), char, prg) {
    if (str_index(prg, "cmarket start") != -1) {
      int ix = str_cindex(prg, ' ');
      if (ix != -1) {
        char *pid = str_left(prg, ix);
        if (rs_get(sys_cmd(str_f("kill %s", pid)))) ok = 1;
      }
      break;
    }
  }_EACH

  db_unlock();

  if (ok) puts("cmarket stoped");
  else puts("cmarket could not be stopped");
}
