// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Avalue.h"

inline
Avalue *avalue_new(void) {
  return arr_new();
}

inline
void avalue_add(Avalue *this, Value *t) {
  arr_add(this, t);
}

inline
Value *avalue_get(Avalue *this, int ix) {
  return arr_get(this, ix);
}

Json *avalue_serialize(Avalue *this) {
  Arr/*Json*/ *r = arr_new();
  EACH(this, Value, t) {
    arr_add(r, value_serialize(t));
  }_EACH
  return json_warray(r);
}

Avalue *avalue_restore(Json *j) {
  Avalue *r = arr_new();
  EACH(json_rarray(j), char, js) {
    avalue_add(r, value_restore(js));
  }_EACH
  return r;
}
