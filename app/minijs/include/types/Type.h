// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Types management.
/// <p>
/// Fields:
///   is_function: It type is a function
///   id: Type identifier
///   params: Arguments in functions and generics otherwise.
#ifndef TYPES_TYPE_H
  # define TYPES_TYPE_H

#include "stdbool.h"
#include "types/Atype.h"

/*.-.*/

#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"

///
typedef struct type_Type Type;

///
Type *type_new(bool is_function, char *id, Atype *params);

///
bool type_is_function(Type *this);

///
char *type_id(Type *this);

///
Atype *type_params(Type *this);

///
Json *type_to_json(Type *this);

///
Type *type_from_json(Json *s);

/*.-.*/

bool type_eq(Type *this, Type *other);


#endif

