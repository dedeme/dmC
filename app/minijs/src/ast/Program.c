// Copyright 29-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Program.h"

static Program *prg = NULL;

Program *program_get(void) {
  if (!prg) {
    prg = arr_new();
  }
  return prg;
}

inline
void program_add(Program *this, Class *class) {
  arr_add(this, class);
}

Class *program__class(Program *this, char *id) {
  EACH(this, Class, c) {
    if (!strcmp(class_id(c), id)) {
      return c;
    }
  }_EACH
  return NULL;
}
