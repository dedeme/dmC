// Copyright 27-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Lsymbol.h"

inline
Lsymbol *lsymbol_new(void) {
  return list_new();
}

inline
Lsymbol *lsymbol_cons(Lsymbol *this, Symbol *s) {
  return list_cons(this, s);
}

Sym *lsymbol__get(Lsymbol *this, char *id) {
  if (list_empty(this)) {
    return NULL;
  }
  Symbol *s = list_head(this);
  if (!strcmp(symbol_id(s), id)) {
    return sym_new(symbol_type(s), symbol_is_val(s));
  }
  return lsymbol__get(list_tail(this), id);
}
