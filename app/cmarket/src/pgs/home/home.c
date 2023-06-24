// Copyright 01-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "pgs/home/home.h"
#include "pgs.h"
#include "db/log.h"

// mrq is Map<char>
void home_process (Map *mrq) {
  char *rq = pgs_rs(mrq, "rq");
  if (str_eq(rq, "getLog")) {
    //<char>
    Map *rp = map_new();
    map_add(rp, "log", log_readJs());
    pgs_print(rp);
  } else if (str_eq(rq, "resetLog")) {
    log_reset();
    pgs_print_empty();
  } else {
    EXC_GENERIC(str_f(
      "Value '%s' not found for 'rq' in '%s'", rq, js_wo(mrq)
    ));
  }
}
