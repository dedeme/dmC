// Copyright 13-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "data/imp.h"
#include "kut/js.h"

Imp *imp_new (int ln, char *mpath, char *id) {
  Imp *this = MALLOC(Imp);
  this->ln = ln;
  this->mpath = mpath;
  this->id = id;
  return this;
}

char *imp_to_js (Imp *this) {
  return js_wa(arr_new_from(
    js_wi(this->ln),
    js_ws(this->mpath),
    js_ws(this->id),
    NULL
  ));
}

///
Imp *imp_from_js (char *js) {
  char **a = (char **)arr_begin(js_ra(js));
  return imp_new (
    js_ri(a[0]),
    js_rs(a[1]),
    js_rs(a[2])
  );
}
