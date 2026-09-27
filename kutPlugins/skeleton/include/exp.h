// Copyright 02-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef EXP_H
  #define EXP_H

#include <stdint.h>
#include "kut/arr.h"
#include "kut/map.h"
#include "kut/tp.h"

/// Kut exception.
#define EXC_KUT(msg) \
    THROW("kut plugin", msg)

///
typedef struct exp_Exp Exp;

/// Returns an empty expression for using with functions which not have return.
Exp *exp_empty (void);

/// Returns TRUE if 'this' is an empty expression.
int exp_is_empty (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_bool (int value);

/// Read an Exp of the indicate type.
/// Throws EXC_KUT if 'this' is not of such type.
int exp_get_bool (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_bool (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_int (int64_t value);

/// Read an Exp of the indicate type.
/// Throws EXC_KUT if 'this' is not of such type.
int64_t exp_get_int (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_int (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_float (double value);

/// Read an Exp of the indicate type.
/// Throws EXC_KUT if 'this' is not of such type.
double exp_get_float (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_float (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_string (char *value);

/// Read an Exp of the indicate type.
/// Throws EXC_KUT if 'this' is not of such type.
char *exp_get_string (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_string (Exp *this);

/// Creates an expression of the indicated type.
/// Types mut be of the form "<symbol>" (e.g. <file>).
Exp *exp_object (char *type, void *value);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
void *exp_get_object (char *type, Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_object (char *type, Exp *this);

/// Returns TRUE if 'this' is an object of undefined type.
int exp_is_some_object (Exp *this);

/// Read an Exp of the indicate type. Returns a Tp<char, void>
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_object_tuple (Exp *this);

/// Creates an expression of the indicated type. 'value' is type Arr<Exp>.
Exp *exp_array (Arr *value);

/// Read an Exp of the indicate type. Returns an array of type Arr<Exp>.
/// Throws EXC_KUT if 'this' is not of such type.
Arr *exp_get_array (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_array (Exp *this);

/// Creates an expression of the indicated type. 'value' is type Map<Exp>.
Exp *exp_dic (Map *value);

/// Read an Exp of the indicate type. Returns an array of type Map<Exp>.
/// Throws EXC_KUT if 'this' is not of such type.
Map *exp_get_dic (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_dic (Exp *this);

/// Returns a string representation of 'this' type.
char *exp_type_to_str (Exp *this);

/// Returns a string representation of this.
/// Differences with exp_to_js are:
///   - string are witout quotes ("abc" -> abc)
///   - float can be without decimal point (3 -> 3)
char *exp_to_str (Exp *this);

/// Returns a JSON string representation of this.
/// Differences with exp_to_str are:
///   - string are between quotes ("abc" -> "abc")
///   - float take ever decimal point (3 -> 3.0)
char *exp_to_js (Exp *this);

#endif
