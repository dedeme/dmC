// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Astat.h"

inline
Astat *astat_new(void) {
  return arr_new();
}

inline
void astat_add(Astat *this, Stat *t) {
  arr_add(this, t);
}

inline
Stat *astat_get(Astat *this, int ix) {
  return arr_get(this, ix);
}

Json *astat_serialize(Astat *this) {
  Arr/*Json*/ *r = arr_new();
  EACH(this, Stat, t) {
    arr_add(r, stat_serialize(t));
  }_EACH
  return json_warray(r);
}

Astat *astat_restore(Json *j) {
  Astat *r = arr_new();
  EACH(json_rarray(j), char, js) {
    astat_add(r, stat_restore(js));
  }_EACH
  return r;
}
