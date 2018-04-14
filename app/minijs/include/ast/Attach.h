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
enum Attach_t attach_type(Attach *this);

///
char *attach_id(Attach *this);

///
Arr *attach_values(Attach *this);

///
Json *attach_serialize(Attach *this);

///
Attach *attach_restore(Json *s);

/*.-.*/

///
Attach *attach_new_dot(char *id);

///
Attach *attach_new_sub(Value *v);

///
Attach *attach_new_fn(Arr/*Value*/ *vs);

#endif
