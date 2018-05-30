// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Mtype.h"

inline
Mtype *mtype_new(void) {
  return map_new();
}

inline
void mtype_put(Mtype *this, char *id, Type *t) {
  map_put(this, id, t);
}

inline
Type *mtype__get(Atype *this, char *id) {
  return map_has_key(this, id) ? map_get(this, id) : NULL;
}
