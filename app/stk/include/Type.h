// Copyright 16-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef TYPE_H
  #define TYPE_H

/// Type class

#include "dmc/std.h"
#include "Token.h"

///
enum Kind {
  BOOL //, BYTE, INT, FLOAT, STRING, ARRAY, MAP, TUPLE, RECORD, FUNCTION, ANY
};

///
typedef struct type_Type Type;

///
void type_free(Type *this);

///
enum Kind type_kind(Type *this);

///
Type *type_bool_new ();

/// Generates a type from a char *. Templates:
///   @: s :
///     Basic type. They are 'b' 'y' 'i' 'f' 's' for bool, byte, int, float
///     and string.
///   @: Name :
///     User type. They are identifiers.
///   @: 'a :
///     Generic type. They start with ' and continue with a identifier.
///   @: [ T ] :
///     Array type. T stands by a type.
///   @: [* T ] :
///     Map type. T stands by a type.
///   @: ( T* ) :
///     Tuple type. T* estands for none, one or several types separated by ','.
///   @: (* I = T* ) :
///     Record type. I = T* estands for none, one or several pairs identifier
///     type separated by ','. Each idenfier must be different to each other.
///   @: { T* > U* } :
///     Function type. T* and U* stands for none, one or several types
///     separated by ','.
/// 't' is Arr[Token]
TkError *type_from (Type **new, Arr *t);

#endif
