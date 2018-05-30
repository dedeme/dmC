// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Mchar.h"

Mchar *mchar_new(void) {
  return (Mchar *)map_new();
}

inline
size_t mchar_size(Mchar *this) {
  return arr_size((Arr *)this);
}

bool  mchar_contains(Mchar *this, char *key) {
  EACH((Arr *)this, Kv, kv) {
    if (!strcmp(kv->key, key)) {
      return true;
    }
  }_EACH
  return false;
}

inline
void mchar_put(Mchar *this, char *key, char *value) {
  return map_put((Map *)this, key, value);
}

inline
char *mchar_get(Mchar *this, char *key) {
  return map_oget((Map*) this, key, key);
}

Arr/*Json*/ *mchar_serialize(Mchar *this) {
  Arr/*Json*/ *r = arr_new();
  EACH((Arr *)this, Kv, kv) {
    arr_add(r, json_wstring(kv->key));
    arr_add(r, json_wstring(kv->value));
  }_EACH
  return r;
}

Mchar *mchar_restore(Arr/*Json*/ *js) {
  Mchar *r = mchar_new();
  size_t size = arr_size(js);
  size_t i = 0;
  while (i < size) {
    char *k = json_rstring(arr_get(js, i++));
    char *v = json_rstring(arr_get(js, i++));
    arr_add((Arr *)r, kv_new(k, v));
  }
  return r;
}
