// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Class attribute type

#ifndef AST_ATT_H
  # define AST_ATT_H

#include <dmc/all.h>
#include "ast/Type.h"
#include "Value.h"

/*.-.*/

///
typedef struct att_Att Att;

///
Att *att_new(
  char *id,
  Type *type,
  bool is_val,
  Value *_value
);

///
char *att_id(Att *this);

///
Type *att_type(Att *this);

///
bool att_is_val(Att *this);

///
Json *att_serialize(Att *this);

///
Att *att_restore(Json *s);

/*.-.*/


///
Value *att_value(Att *this);

#endif

