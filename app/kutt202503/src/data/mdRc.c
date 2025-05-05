// Copyright 16-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "data/mdRc.h"
#include "kut/js.h"
#include "data/tp.h"
#include "data/imp.h"
#include "data/typedSym.h"
#include "data/type.h"

MdRc *mdRc_new(
  char *path, char *id, Map *imports, Arr *exports, Map *type_defs
) {
  MdRc *this = MALLOC(MdRc);
  this->path = path;
  this->id = id;
  this->imports = imports;
  this->exports = exports;
  this->type_defs = type_defs;
  return this;
}

char *mdRc_to_js (MdRc *this) {
  char *tptp_to_js (Tp *t) {
    return js_wa(arr_new_from(
      type_to_js(t->e1),
      type_to_js(t->e2),
      NULL
    ));
  }
  return js_wa(arr_new_from(
    js_ws(this->path),
    js_ws(this->id),
    map_to_js(this->imports, (FTO)imp_to_js),
    arr_to_js(this->exports, (FTO)typedSym_to_js),
    map_to_js(this->type_defs, (FTO)tptp_to_js),
    NULL
  ));
}

MdRc *mdRc_from_js (char *js) {
  // Tp<Type, Type>
  Tp *tptp_from_js (char *js) {
    char **a = (char **)arr_begin(js_ra(js));
    return tp_mk(
      type_from_js(a[0]),
      type_from_js(a[1])
    );
  }
  char **a = (char **)arr_begin(js_ra(js));
  return mdRc_new(
    js_rs(a[0]),
    js_rs(a[1]),
    map_from_js(a[2], (FFROM)imp_from_js),
    arr_from_js(a[3], (FFROM)typedSym_from_js),
    map_from_js(a[4], (FFROM)tptp_from_js)
  );
}
