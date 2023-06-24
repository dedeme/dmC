// Copyright 05-06-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/cosT.h"
#include "kut/DEFS.h"
#include "kut/js.h"
#include "data/co.h"

CosT *cosT_new (char *main, Arr *cos) {
  CosT *this = MALLOC(CosT);
  this->main = main;
  this->cos = cos;
  return this;
}

char *cosT_to_js (CosT *this) {
  return js_wa(arr_new_from(
    js_ws(this->main),
    arr_to_js(this->cos, (FTO)co_to_js),
    NULL
  ));
}

CosT *cosT_from_js (char *js) {
  Arr *a = js_ra(js);
  return cosT_new(
    js_rs(arr_get(a, 0)),
    arr_from_js(arr_get(a, 1), (FFROM)co_from_js)
  );
}
