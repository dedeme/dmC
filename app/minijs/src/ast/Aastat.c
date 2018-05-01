// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Aastat.h"

inline
Aastat *aastat_new(void) {
  return arr_new();
}

inline
void aastat_add(Aastat *this, Astat *a) {
  arr_add(this, a);
}

inline
Astat *aastat_get(Aastat *this, int ix) {
  return arr_get(this, ix);
}

Json *aastat_serialize(Aastat *this) {
  Arr/*Json*/ *r = arr_new();
  EACH(this, Astat, a) {
    arr_add(r, astat_serialize(a));
  }_EACH
  return json_warray(r);
}

Aastat *aastat_restore(Json *j) {
  Aastat *r = arr_new();
  EACH(json_rarray(j), char, js) {
    aastat_add(r, astat_restore(js));
  }_EACH
  return r;
}
