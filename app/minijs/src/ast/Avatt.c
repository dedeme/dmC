// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Avatt.h"

inline
Avatt *avatt_new(void) {
  return arr_new();
}

inline
void avatt_add(Avatt *this, Vatt *t) {
  arr_add(this, t);
}

inline
Vatt *avatt_get(Avatt *this, int ix) {
  return arr_get(this, ix);
}

Json *avatt_serialize(Avatt *this) {
  Arr/*Json*/ *r = arr_new();
  EACH(this, Vatt, t) {
    arr_add(r, vatt_serialize(t));
  }_EACH
  return json_warray(r);
}

Avatt *avatt_restore(Json *j) {
  Avatt *r = arr_new();
  EACH(json_rarray(j), char, js) {
    avatt_add(r, vatt_restore(js));
  }_EACH
  return r;
}
