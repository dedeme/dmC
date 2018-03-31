// Copyright 25-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// AST value declaration model
/// <h3>Parameters note:</h3>
/// <dl>
/// <dt><tt>tpos</tt><dt><dd>Initialized to filepos_new(pos, 0, 0)</dd>
/// <dt><tt>type</tt><dt>
/// <dd>Initialized to type_new(UNKNOWN, "", arr_new())</dd>
/// <dt><tt>value</tt><dt><dd>Initialized to value_new_null()</dd>

#ifndef AST_DVALUE_H
  #define AST_DVALUE_H

#include <dmc/all.h>
#include "types/Type.h"
#include "ast/Value.h"
#include "lexer/Pos.h"

/*.-.*/

///
typedef struct dvalue_Dvalue Dvalue;

///
Pos *dvalue_pos(Dvalue *this);

///
char *dvalue_id(Dvalue *this);

///
Pos *dvalue_tpos(Dvalue *this);

///
void dvalue_set_tpos(Dvalue *this, Pos *value);

///
Type *dvalue_type(Dvalue *this);

///
void dvalue_set_type(Dvalue *this, Type *value);

///
Value *dvalue_value(Dvalue *this);

///
void dvalue_set_value(Dvalue *this, Value *value);

///
Json *dvalue_serialize(Dvalue *this);

///
Dvalue *dvalue_restore(Json *s);

/*.-.*/

Dvalue *dvalue_new(Pos *pos, char *id);

#endif
