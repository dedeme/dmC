// Copyright 13-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "parser/Obj.h"

/*.
-struct: Obj
  absolute_id: char *
  type: Type *
  pos: Pos *
  is_public: bool
  is_static: bool
  is_val: bool
*/

/*.-.*/
struct obj_Obj {
  char *absolute_id;
  Type *type;
  Pos *pos;
  bool is_public;
  bool is_static;
  bool is_val;
};

Obj *obj_new(
  char *absolute_id,
  Type *type,
  Pos *pos,
  bool is_public,
  bool is_static,
  bool is_val
) {
  Obj *this = MALLOC(Obj);
  this->absolute_id = absolute_id;
  this->type = type;
  this->pos = pos;
  this->is_public = is_public;
  this->is_static = is_static;
  this->is_val = is_val;
  return this;
}

inline
char *obj_absolute_id(Obj *this) {
  return this->absolute_id;
}

inline
Type *obj_type(Obj *this) {
  return this->type;
}

inline
Pos *obj_pos(Obj *this) {
  return this->pos;
}

inline
bool obj_is_public(Obj *this) {
  return this->is_public;
}

inline
bool obj_is_static(Obj *this) {
  return this->is_static;
}

inline
bool obj_is_val(Obj *this) {
  return this->is_val;
}
/*.-.*/
