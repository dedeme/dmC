// Copyright 13-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "parser/Lobj.h"
#include "lexer/Txpos.h"

/*.
-struct: Lobj
  id: char *
  class: Final_class *
  type: Type *
  pos: Pos *
  is_val: bool

-struct: @Lobjs
  up: List *
  current: List *
*/

/*.-.*/
struct lobj_Lobj {
  char *id;
  Final_class *class;
  Type *type;
  Pos *pos;
  bool is_val;
};

Lobj *lobj_new(
  char *id,
  Final_class *class,
  Type *type,
  Pos *pos,
  bool is_val
) {
  Lobj *this = MALLOC(Lobj);
  this->id = id;
  this->class = class;
  this->type = type;
  this->pos = pos;
  this->is_val = is_val;
  return this;
}

inline
char *lobj_id(Lobj *this) {
  return this->id;
}

inline
Final_class *lobj_class(Lobj *this) {
  return this->class;
}

inline
Type *lobj_type(Lobj *this) {
  return this->type;
}

inline
Pos *lobj_pos(Lobj *this) {
  return this->pos;
}

inline
bool lobj_is_val(Lobj *this) {
  return this->is_val;
}

struct lobjs_Lobjs {
  List *up;
  List *current;
};

Lobjs *_lobjs_new(List *up, List *current) {
  Lobjs *this = MALLOC(Lobjs);
  this->up = up;
  this->current = current;
  return this;
}

inline
List *lobjs_up(Lobjs *this) {
  return this->up;
}

inline
List *lobjs_current(Lobjs *this) {
  return this->current;
}
/*.-.*/

inline
Lobjs *lobjs_new(void) {
  return _lobjs_new(list_new(), list_new());
}

static Lobj *get(List/*Lobj*/ *list, char *id) {
  EACHL(list, Lobj, o) {
    if (!strcmp(o->id, id)) {
      return o;
    }
  }_EACH
  return NULL;
}

Lobjs *lobjs_add(Lobjs *this, Lobj *obj) {
  Lobj *old = get(this->current, obj->id);
  if (old)
    TH3(obj->class, obj->pos)
      "'%s' already was defined here: %s[%d, %d]",
      obj->id, cpath_file(final_class_cpath(old->class)),
      pos_nline(old->pos), pos_nchar(old->pos)
    _TH
  return _lobjs_new(this->up, list_cons(this->current, obj));
}

Lobjs *lobjs_add_up(Lobjs *this) {
  List/*Lobjs*/ *current = this->current;
  List/*Lobjs*/ *newl = current;
  EACHL(this->up, Lobj, o) {
    if (!get(current, o->id)) {
      newl = list_cons(newl, o);
    }
  }_EACH
  return _lobjs_new(newl, list_new());
}

Lobj *lobjs_get(Lobjs *this, char *id) {
  Lobj *o = get(this->current, id);
  return o ? o : get(this->up, id);
}
