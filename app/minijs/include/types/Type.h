// Copyright 24-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Object to encapsulate a type.<br>
/// Types are <b>nullable</b> objects.
/// <h3>Generics notes:</h3>
/// Function 'type_params()' has the folowing complete definition:<br>
/// <tt><b>Arr[Type]</b> *type_params(Type *this)</tt><br>
/// The last element of Arr[type] can be <b>null</b> if Type is <i>function</i>.

#ifndef TYPES_TYPE_H
  # define TYPES_TYPE_H

#include <dmc/all.h>
#include "DEFS.h"

/*.-.*/

///
typedef struct type_Type Type;

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

///
Type *type_new_unknown();

///
Type *type_new_bool();

///
Type *type_new_byte();

///
Type *type_new_int();

///
Type *type_new_float();

///
Type *type_new_char();

///
Type *type_new_str();

///
Type *type_new_any();

///
Type *type_new_arr(Type *t);

///
Type *type_new_map(Type *t);

/// type_new_fn creates a function type. The las field of 'ts' is the return
/// type. If there is not return its value is NULL.
Type *type_new_fn(Arr/*Type*/ *ts);

///
Type *type_new_data(char *id, Arr/*Type*/ *generics);

///
bool type_eq(Type *this, Type *other);

///
bool type_is_unknown(Type *this);

///
char *type_to_str(Type *this);

#endif
