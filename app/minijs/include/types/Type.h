// Copyright 24-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Object to encapsulate a type.<br>
/// <h3>Generics notes:</h3>
/// <ul>
/// <li><tt>Type *type_new(enum Type_t type, char *id,
/// <b>Arr[Type]</b> *params)</tt></li>
/// <li><tt><b>Arr[Type]</b> *type_params(Type *this)</tt></li>
/// </ul>

#ifndef TYPES_TYPE_H
  # define TYPES_TYPE_H

#include <dmc/all.h>
#include "DEFS.h"
#include "lexer/Txpos.h"

/*.-.*/

///
typedef struct type_Type Type;

///
Type *type_new(enum Type_t type, char *id, Arr *params);

///
enum Type_t type_type(Type *this);

///
char *type_id(Type *this);

///
Arr *type_params(Type *this);

///
Json *type_serialize(Type *this);

///
Type *type_restore(Json *s);

/*.-.*/

bool type_eq(Type *this, Type *other);

#endif
