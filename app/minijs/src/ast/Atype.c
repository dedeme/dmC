// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Atype.h"

inline
Atype *atype_new(void) {
  return arr_new();
}

inline
void atype_add(Atype *this, Type *t) {
  arr_add(this, t);
}

inline
Type *atype_get(Atype *this, int ix) {
  return arr_get(this, ix);
}

Arr/*Json*/ *atype_serialize(Atype *this) {
  Arr/*Json*/ *r = arr_new();
  EACH(this, Type, t) {
    arr_add(r, json_warray(type_serialize(t)));
  }_EACH
  return r;
}

Atype *atype_restore(Arr/*Json*/ *j) {
  Atype *r = arr_new();
  EACH(j, char, js) {
    atype_add(r, type_restore(json_rarray(js)));
  }_EACH
  return r;
}
