// Copyright 13-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Local object

#ifndef PARSER_LOBJ_H
  # define PARSER_LOBJ_H

#include <dmc/all.h>
#include "types/Type.h"
#include "lexer/Pos.h"
#include "Final_class.h"

/*.-.*/

///
typedef struct lobj_Lobj Lobj;

///
Lobj *lobj_new(
  char *id,
  Final_class *class,
  Type *type,
  Pos *pos,
  bool is_val
);

///
char *lobj_id(Lobj *this);

///
Final_class *lobj_class(Lobj *this);

///
Type *lobj_type(Lobj *this);

///
Pos *lobj_pos(Lobj *this);

///
bool lobj_is_val(Lobj *this);


///
typedef struct lobjs_Lobjs Lobjs;

///
List *lobjs_up(Lobjs *this);

///
List *lobjs_current(Lobjs *this);

/*.-.*/

///
Lobjs *lobjs_new(void);

/// lobjs_add returns a new Lobjs with 'obj' added to lobjs_current.<br>
/// If one Lobj with the same id as 'obj' already exists in lobjs_current,
/// it throws a tx_exception.
Lobjs *lobjs_add(Lobjs *this, Lobj *obj);

/// lobjs_add_up returns a new Lobjs with lobjs_current added to lobjs_up and
/// lojbs_current empty.
Lobjs *lobjs_add_up(Lobjs *this);

/// lobjs_get returns the Lobj with name 'id'. It searchs first in lobjs_current
/// and then in lobjs_up. If id is not found return NULL.
Lobj *lobjs_get(Lobjs *this, char *id);

#endif
