// Copyright 27-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Sym.h"

/*.
-struct: Sym
  type: Type *
  is_val: bool
*/

/*.-.*/
struct sym_Sym {
  Type *type;
  bool is_val;
};

Sym *sym_new(Type *type, bool is_val) {
  Sym *this = MALLOC(Sym);
  this->type = type;
  this->is_val = is_val;
  return this;
}

inline
Type *sym_type(Sym *this) {
  return this->type;
}

inline
bool sym_is_val(Sym *this) {
  return this->is_val;
}
/*.-.*/
