// Copyright 17-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Aachar.h"

inline
Aachar *aachar_new(void) {
  return (Aachar *)arr_new();
}

inline
size_t aachar_size(Aachar *this) {
  return arr_size((Arr *)this);
}

inline
void aachar_add(Aachar *this, Achar *a) {
  arr_add((Arr *)this, a);
}

inline
Achar *aachar_get(Aachar *this, int ix) {
  return arr_get((Arr *)this, ix);
}

Arr/*Json*/ *aachar_serialize(Aachar *this) {
  Arr/*Json*/ *r = arr_new();
  EACH((Arr *)this, Achar, a) {
    arr_add(r, json_warray(achar_serialize(a)));
  }_EACH
  return r;
}

Aachar *aachar_restore(Arr/*Json*/ *j) {
  Aachar *r = aachar_new();
  EACH(j, char, js) {
    aachar_add(r, achar_restore(json_rarray(js)));
  }_EACH
  return r;
}
