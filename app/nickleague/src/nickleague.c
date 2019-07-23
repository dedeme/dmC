// Copyright 17-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "nickleague.h"
#include "log.h"
#include "io.h"
#include "League.h"

int main (int argc, char *argv[]) {
  sys_init("nickleague");

  TRY
    // Arr[char]
    Arr *nicks = arr_new();
    // Arr[Darr]
    Arr *quotes = arr_new();
    // Arr[char]
    Arr *nks = io_nicks();
    arr_shuffle(nks);
    EACH(nks, char, nick)
      Darr *qs = io_quotes(nick);
      if (darr_size(qs)) {
        arr_push(nicks, nick);
        arr_push(quotes, qs);
      }
    _EACH

    if (!arr_size(nicks))
      EXC_ILLEGAL_STATE("No nicks read")

    log_ok(league_to_js(league_new(nicks, quotes)));

  CATCH(e)
    log_error(str_f("%s\n%s", exc_msg(e), str_cjoin(exc_stack(e), '\n')));
    return 1;
  _TRY


  return 0;
}
