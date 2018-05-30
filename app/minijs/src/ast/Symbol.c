// Copyright 27-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Symbol.h"

/*.
-struct: Symbol
  id: char *
  type: Type *
  is_val: bool
*/

/*.-.*/
struct symbol_Symbol {
  char *id;
  Type *type;
  bool is_val;
};

Symbol *symbol_new(char *id, Type *type, bool is_val) {
  Symbol *this = MALLOC(Symbol);
  this->id = id;
  this->type = type;
  this->is_val = is_val;
  return this;
}

inline
char *symbol_id(Symbol *this) {
  return this->id;
}

inline
Type *symbol_type(Symbol *this) {
  return this->type;
}

inline
bool symbol_is_val(Symbol *this) {
  return this->is_val;
}
/*.-.*/
