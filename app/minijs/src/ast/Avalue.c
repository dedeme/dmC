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

Arr/*Json*/ *avalue_serialize(Avalue *this) {
  Arr/*Json*/ *r = arr_new();
  EACH(this, Value, t) {
    arr_add(r, json_warray(value_serialize(t)));
  }_EACH
  return r;
}

Avalue *avalue_restore(Arr/*Json*/ *j) {
  Avalue *r = arr_new();
  EACH(j, char, js) {
    avalue_add(r, value_restore(json_rarray(js)));
  }_EACH
  return r;
}
