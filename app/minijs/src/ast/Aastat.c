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

Arr/*Json*/ *aastat_serialize(Aastat *this) {
  Arr/*Json*/ *r = arr_new();
  EACH(this, Astat, a) {
    arr_add(r, json_warray(astat_serialize(a)));
  }_EACH
  return r;
}

Aastat *aastat_restore(Arr/*Json*/ *j) {
  Aastat *r = arr_new();
  EACH(j, char, js) {
    aastat_add(r, astat_restore(json_rarray(js)));
  }_EACH
  return r;
}
