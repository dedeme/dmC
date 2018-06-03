// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Aatt.h"

inline
Aatt *aatt_new(void) {
  return (Aatt *)arr_new();
}

inline
size_t aatt_size(Aatt *this) {
  return arr_size((Arr *) this);
}

inline
void aatt_add(Aatt *this, Att *t) {
  arr_add((Arr *)this, t);
}

inline
Att *aatt_get(Aatt *this, int ix) {
  return arr_get((Arr *)this, ix);
}

Att *aatt__get(Aatt *this, char *id) {
  EACH((Arr *)this, Att, a) {
    if (!strcmp(att_id(a), id)) {
      return a;
    }
  }_EACH
  return NULL;
}

inline
void aatt_set(Aatt *this, int ix, Att *a) {
  arr_set((Arr *)this, ix, a);
}

Arr/*Json*/ *aatt_serialize(Aatt *this) {
  Arr/*Json*/ *r = arr_new();
  EACH((Arr *)this, Att, t) {
    arr_add(r, json_warray(att_serialize(t)));
  }_EACH
  return r;
}

Aatt *aatt_restore(Arr/*Json*/ *j) {
  Aatt *r = aatt_new();
  EACH(j, char, js) {
    aatt_add(r, att_restore(json_rarray(js)));
  }_EACH
  return r;
}
