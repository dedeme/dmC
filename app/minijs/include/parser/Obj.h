// Copyright 13-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Class object

#ifndef PARSER_OBJ_H
  # define PARSER_OBJ_H

#include <dmc/all.h>
#include "types/Type.h"
#include "lexer/Pos.h"

/*.-.*/

///
typedef struct obj_Obj Obj;

///
Obj *obj_new(
  Type *type,
  Pos *pos,
  bool is_public,
  bool is_static,
  bool is_val
);

///
Type *obj_type(Obj *this);

///
Pos *obj_pos(Obj *this);

///
bool obj_is_public(Obj *this);

///
bool obj_is_static(Obj *this);

///
bool obj_is_val(Obj *this);

/*.-.*/

#endif
