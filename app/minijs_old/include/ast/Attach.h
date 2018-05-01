// Copyright 25-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Velue attachment model. It Reads following additions:
///   Point -> .identifier. For methods and properties
///   square brackets -> [index]. For subindex of arrays and maps.
///   parenthesis -> (a, b, c ...) or (). For functions

#ifndef AST_ATTACH_H
  #define AST_ATTACH_H

#include <dmc/all.h>
#include "DEFS.h"
#include "ast/Value.h"

/*.-.*/

///
typedef struct attach_Attach Attach;

///
Pos *attach_pos(Attach *this);

///
enum Attach_t attach_type(Attach *this);

///
char *attach_id(Attach *this);

///
void attach_set_id(Attach *this, char *value);

///
Arr *attach_values(Attach *this);

///
Json *attach_serialize(Attach *this);

///
Attach *attach_restore(Json *s);

/*.-.*/

///
Attach *attach_new_dot(Pos *pos, char *id);

///
Attach *attach_new_sub(Pos *pos, Value *v);

///
Attach *attach_new_fn(Pos *pos, Arr/*Value*/ *vs);

#endif
