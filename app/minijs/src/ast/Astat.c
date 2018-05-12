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

Arr/*Json*/ *astat_serialize(Astat *this) {
  Arr/*Json*/ *r = arr_new();
  EACH(this, Stat, t) {
    arr_add(r, json_warray(stat_serialize(t)));
  }_EACH
  return r;
}

Astat *astat_restore(Arr/*Json*/ *j) {
  Astat *r = arr_new();
  EACH(j, char, js) {
    astat_add(r, stat_restore(json_rarray(js)));
  }_EACH
  return r;
}
