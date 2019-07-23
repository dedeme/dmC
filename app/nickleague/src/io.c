// Copyright 17-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "io.h"
#include "DEFS.h"

Arr *io_nicks (void) {
  // Arr[Js]
  Arr *nicks_db = js_ra((Js *)file_read(NICKS_FILE));
  // Arr[Js]
  Arr *nicks = js_ra(arr_get(nicks_db, 2));
  // Arr[char]
  Arr *r = arr_new();
  EACH(nicks, Js, js)
    // Arr[char]
    Arr *fields = js_ra(js);
    if (js_rb(arr_get(fields, 2))) {
      arr_push(r, js_rs(arr_get(fields, 1)));
    }
  _EACH

  return r;
}

Darr *io_quotes (char *nick) {
  // Arr[Js]
  Arr *qs_db = js_ra((Js *)file_read(str_f("%s/%s", QUOTES_DIR, nick)));
  // Arr[Js]
  Arr *qs = js_ra(arr_get(qs_db, 1));
  Darr *r = darr_new();
  EACH(qs, Js, js)
    // Arr[Js]
    Arr *fields = js_ra(js);
    darr_push(r, js_rd(arr_get(fields, 1)));
  _EACH

  return r;
}
