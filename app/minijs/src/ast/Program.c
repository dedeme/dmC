// Copyright 29-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Program.h"
#include "builtin/tall.h"
#include "lexer/rclass.h"
#include "parser/pclass.h"
#include "parser/pattribute.h"

static Program *prg = NULL;

Program *program_get(void) {
  if (!prg) {
    prg = (Program *)arr_new();
    tall_add((Arr *) prg);
  }
  return prg;
}

size_t program_size(Program *this) {
  return arr_size((Arr *)this);
}

inline
void program_add(Program *this, Class *class) {
  arr_add((Arr *)this, class);
}

Class *program__class(Program *this, char *id) {
  EACH((Arr *)this, Class, c) {
    if (!strcmp(class_id(c), id)) {
      return c;
    }
  }_EACH
  return NULL;
}

Type *patt(Class *c, Atype *generics, char *id) {

  return type_new_unknown();
}

Sym *program_att_main(char *c_id, Atype *generics) {
  Class *c = program__class(program_get(), c_id);
  if (!c) {
    c = rclass(cpath_from_id(c_id));
    pclass_main(c, generics);
    program_add(program_get(), c);
    pclass_implementation(c, generics);
  }

  return pattribute(c, "main", generics, true, true);
}

Type *program_att(Class *caller, char *c_id, Atype *generics, char *id) {
  Class *c = program__class(program_get(), c_id);
  if (!c) {
    c = rclass(cpath_from_id(c_id));
    pclass(caller, c, generics);
    program_add(program_get(), c);
    pclass_implementation(c, generics);
  }

  return patt(c, generics, id);
}
