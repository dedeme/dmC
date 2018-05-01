// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Mchar.h"

Mchar *mchar_new(void) {
  return map_new();
}

bool  mchar_contains(Mchar *this, char *key) {
  EACH(this, Kv, kv) {
    if (!strcmp(kv->key, key)) {
      return true;
    }
  }_EACH
  return false;
}

inline
void mchar_put(Mchar *this, char *key, char *value) {
  return map_put(this, key, value);
}

inline
char *mchar_get(Mchar *this, char *key) {
  return map_oget(this, key, key);
}

Json *mchar_serialize(Mchar *this) {
  Arr/*Json*/ *r = arr_new();
  EACH(this, Kv, kv) {
    arr_add(r, json_wstring(kv->key));
    arr_add(r, json_wstring(kv->value));
  }_EACH
  return json_warray(r);
}

Mchar *mchar_restore(Json *j) {
  Arr/*Kv*/ *r = arr_new();
  Arr/*Json*/ *js = json_rarray(j);
  size_t size = arr_size(js);
  size_t i = 0;
  while (i < size) {
    char *k = json_rstring(arr_get(js, i++));
    char *v = json_rstring(arr_get(js, i++));
    arr_add(r, kv_new(k, v));
  }
  return r;
}
