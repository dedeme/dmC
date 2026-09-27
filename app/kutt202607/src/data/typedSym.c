// Copyright 13-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "data/typedSym.h"
#include "kut/js.h"

TypedSym *typedSym_new (
  int ln, char *id, Type *tp, Opt *cval, int used
) {
  TypedSym *this = MALLOC(TypedSym);
  this->ln = ln;
  this->id = id;
  this->tp = tp;
  this->cval = cval;
  this->used = used;
  return this;
}

char *typedSym_to_js (TypedSym *this) {
  return js_wa(arr_new_from(
    js_wi(this->ln),
    js_ws(this->id),
    type_to_js(this->tp),
    opt_to_js(this->cval, (FTO)js_ws),
    js_wi(this->used),
    NULL
  ));
}

TypedSym *typedSym_from_js (char *js) {
  char **a = (char **)arr_begin(js_ra(js));
  return typedSym_new(
    js_ri(a[0]),
    js_rs(a[1]),
    type_from_js(a[2]),
    opt_from_js(a[3], (FFROM)js_rs),
    js_ri(a[4])
  );
}
