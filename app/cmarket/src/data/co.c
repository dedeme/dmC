// Copyright 05-06-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/co.h"
#include "kut/DEFS.h"
#include "kut/js.h"

Co *co_new (char *nick, int is_selected) {
  Co *this = MALLOC(Co);
  this->nick = nick;
  this->is_selected = is_selected;
  return this;
}

char *co_to_js (Co *this) {
  return js_wa(arr_new_from(
    js_ws(this->nick),
    js_wb(this->is_selected),
    NULL
  ));
}

Co *co_from_js (char *js) {
  Arr *a = js_ra(js);
  return co_new(
    js_rs(arr_get(a, 0)),
    js_rb(arr_get(a, 1))
  );
}
