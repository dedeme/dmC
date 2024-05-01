// Copyright 04-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Type data (immtutable).

#ifndef READ_TYPE_H
  #define READ_TYPE_H

#include "kut/arr.h"

///
typedef enum type_type TypeT;
///
typedef struct read_type Type;

///
enum type_type {
  TP_BOOL, TP_INT, TP_FLOAT, TP_STRING, TP_GENERIC, TP_OBJECT,
  TP_TUPLE, TP_ARR, TP_REC, TP_DIC, TP_FUNC
};

///
struct read_type {
  TypeT type;
  void *value;
};

/// Constructor
Type *type_mk_bool ();

/// Constructor
Type *type_mk_int ();

/// Constructor
Type *type_mk_float ();

/// Constructor
Type *type_mk_string ();

/// Constructor
/// 'id' is [A, B, ..., Z]
Type *type_mk_generic (char *id);

/// Returns its identifier of a generic type.
char *type_generic_id (Type *this);

/// Constructor
///   id: Object identifier. NOTE: It can be duplicated!!!
///   generics: Arr<Type> with generics types.
Type *type_mk_object (char *id, Arr *generics);

/// Returns idenfitier of a object type.
char *type_object_id (Type *this);

/// Returns Arr<Type> with generics of a object type.
Arr *type_object_generics (Type *this);

/// Constructor
/// 'types' is Arr<Type>
Type *type_mk_tuple (Arr *types);

/// Returns types of a tuple type.
/// Returns Arr<Type>
Arr *type_tuple_types (Type *this);

/// Constructor.
Type *type_mk_arr (Type *type);

/// Returns type of an array type.
Type *type_arr_type (Type *this);

/// Constructor
/// 'types' is Arr<Type>
Type *type_mk_rec (Arr *types);

/// Returns types of a record type.
/// Returns Arr<Type>
Arr *type_rec_types (Type *this);

/// Constructor.
Type *type_mk_dic (Type *type);

/// Returns type of a dictionary type.
Type *type_dic_type (Type *this);

/// Constructor.
///   parms: Function parameter types. It is Arr<Type>.
///   ret: Function return type. It is Opt<Type>.
Type *type_mk_func (Arr *params, Opt *ret);

/// Returns Arr<Type> with parameters of a function type.
Arr *type_func_params (Type *this);

/// Returns Opt<Type> with return of a function type.
Opt *type_func_return (Type *this);

// -----------------------------------------------------------------------------

/// Returns TRUE if 'this' if of 'requested' type.
int type_ok (Type *this, Type *requested);

/// THROW exc_kut_t
Type *type_from_str (char *s);

#endif
