// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Achar.h"

inline
Achar *achar_new(void) {
  return arr_new();
}

bool achar_contains(Achar *this, char *s) {
  EACH(this, char, e) {
    if (!strcmp(e, s)){
      return true;
    }
  }_EACH
  return false;
}

inline
void achar_add(Achar *this, char *t) {
  arr_add(this, t);
}

inline
char *achar_get(Achar *this, int ix) {
  return arr_get(this, ix);
}

Json *achar_serialize(Achar *this) {
  Arr/*Json*/ *r = arr_new();
  EACH(this, char, s) {
    arr_add(r, json_wstring(s));
  }_EACH
  return json_warray(r);
}

Achar *achar_restore(Json *j) {
  Achar *r = arr_new();
  EACH(json_rarray(j), char, js) {
    achar_add(r, json_rstring(js));
  }_EACH
  return r;
}
