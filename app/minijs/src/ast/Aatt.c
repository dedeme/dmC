// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Aatt.h"

inline
Aatt *aatt_new(void) {
  return arr_new();
}

inline
void aatt_add(Aatt *this, Att *t) {
  arr_add(this, t);
}

inline
Att *aatt_get(Aatt *this, int ix) {
  return arr_get(this, ix);
}

Att *aatt__get(Aatt *this, char *id) {
  EACH(this, Att, a) {
    if (!strcmp(att_id(a), id)) {
      return a;
    }
  }_EACH
  return NULL;
}

Arr/*Json*/ *aatt_serialize(Aatt *this) {
  Arr/*Json*/ *r = arr_new();
  EACH(this, Att, t) {
    arr_add(r, json_warray(att_serialize(t)));
  }_EACH
  return r;
}

Aatt *aatt_restore(Arr/*Json*/ *j) {
  Aatt *r = arr_new();
  EACH(j, char, js) {
    aatt_add(r, att_restore(json_rarray(js)));
  }_EACH
  return r;
}
