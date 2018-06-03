// Copyright 29-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Imported.h"
#include "builtin/tall.h"

static Arr/*Class*/ *builtins = NULL;

/*.
-struct: @Imported
  -data: Class **
  -ix: int
*/

/*.-.*/
struct imported_Imported {
  Class **data;
  int ix;
};

Imported *_imported_new(Class **data, int ix) {
  Imported *this = MALLOC(Imported);
  this->data = data;
  this->ix = ix;
  return this;
}
/*.-.*/

static Imported *im = NULL;

Imported *imported_get(void) {
  if (!im) {
    builtins = arr_new();
    tall_add(builtins);

    Class **classes = GC_MALLOC(100 * sizeof(Class *));
    Class **p = classes;
    REPEAT(100) {
      *p++ = NULL;
    }_REPEAT
    im = _imported_new(classes, 0);
  }
  return im;
}


void imported_add(Imported *this, Class *class) {
  this->data[this->ix++] = class;
  if (this->ix == 100) {
    this->ix = 0;
  }
}

Class *imported__class(Imported *this, char *id) {
  EACH(builtins, Class, c) {
    if (!strcmp(class_id(c), id)) {
      return c;
    }
  }_EACH

  Class **p = this->data;
  REPEAT(100) {
    if (!*p) {
      return NULL;
    }
    if (!strcmp(class_id(*p), id)) {
      return *p;
    }
    ++p;
  }_REPEAT
  return NULL;
}
