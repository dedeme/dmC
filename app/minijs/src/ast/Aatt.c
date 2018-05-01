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

Json *aatt_serialize(Aatt *this) {
  Arr/*Json*/ *r = arr_new();
  EACH(this, Att, t) {
    arr_add(r, att_serialize(t));
  }_EACH
  return json_warray(r);
}

Aatt *aatt_restore(Json *j) {
  Aatt *r = arr_new();
  EACH(json_rarray(j), char, js) {
    aatt_add(r, att_restore(js));
  }_EACH
  return r;
}
