// Copyright 28-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Immutable class of Kut expressions.

#ifndef EXP_H
  #define EXP_H

#include <stdint.h>
#include "kut/arr.h"
#include "kut/map.h"
#include "kut/tp.h"
#include "kut/tp3.h"

typedef struct stack_Stack Stack;

typedef struct function_Function Function;

///
typedef struct exp_Exp Exp;

// CONSTRUCTORS AND GET-TESTS

/// Returns an empty expression for using with functions which not have return.
Exp *exp_empty (void);

/// Returns TRUE if 'this' is an empty expression.
int exp_is_empty (Exp *this);

/// Returns an empty-return expression for using with such type of return.
Exp *exp_empty_return (void);

/// Returns TRUE if 'this' is an empty-return expression.
int exp_is_empty_return (Exp *this);

/// Returns a 'cyclic symbol' expression used to prevent cyclic references.
Exp *exp_cyclic (void);

/// Returns TRUE if 'this' is a cyclic expression.
int exp_is_cyclic (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_break (Stack *value);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Stack *exp_get_break (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_break (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_continue (Stack *value);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Stack *exp_get_continue (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_continue (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_bool (int value);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
int exp_get_bool (Exp *this);

/// Read an Exp of the indicate type in run time.
/// Throws EXC_KUT if 'this' is not of such type.
int exp_rget_bool (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_bool (Exp *this);

/// Read an Exp of type bool, string or array as boolean.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
int exp_rget_as_bool (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_int (int64_t value);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
int64_t exp_get_int (Exp *this);

/// Read an Exp of the indicate type in run time.
/// Throws EXC_KUT if 'this' is not of such type.
int64_t exp_rget_int (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_int (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_float (double value);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
double exp_get_float (Exp *this);

/// Read an Exp of the indicate type in run time.
/// Throws EXC_KUT if 'this' is not of such type.
double exp_rget_float (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_float (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_string (char *value);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
char *exp_get_string (Exp *this);

/// Read an Exp of the indicate type in run time.
/// Throws EXC_KUT if 'this' is not of such type.
char *exp_rget_string (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_string (Exp *this);

/// Creates an expression of the indicated type.
/// Types "<empty expression>", "<empty return>" and "<cyclic symbol>" are
/// reserved.
Exp *exp_object (char *type, void *value);

/// Read an Exp of the indicate type. Returns a Tp<char, void>
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
void *exp_rget_object (char *type, Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_object (char *type, Exp *this);

/// Returns TRUE if 'this' is an object of undefined type.
int exp_is_some_object (Exp *this);

/// Creates an expression of the indicated type. 'value' is type Arr<Exp>.
Exp *exp_array (Arr *value);

/// Read an Exp of the indicate type. Returns an array of type Arr<Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *exp_get_array (Exp *this);

/// Read an Exp of the indicate type in run time. Returns an array of type Arr<Exp>.
/// Throws EXC_KUT if 'this' is not of such type.
Arr *exp_rget_array (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_array (Exp *this);

/// Creates an expression of the indicated type. 'value' is type Map<Exp>.
Exp *exp_dic (Map *value);

/// Read an Exp of the indicate type. Returns an array of type Map<Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Map *exp_get_dic (Exp *this);

/// Read an Exp of the indicate type. Returns an array of type Map<Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Map *exp_rget_dic (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_dic (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_function (Function *value);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Function *exp_get_function (Exp *this);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Function *exp_rget_function (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_function (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_sym (int value);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
int exp_get_sym (Exp *this);

/// Read an Exp of the indicate type in run time.
/// Throws EXC_KUT if 'this' is not of such type.
int exp_rget_sym (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_sym (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_range (Exp *v1, Exp *v2, Exp *v3);

/// Read an Exp of the indicate type. Returns a Tp3<Exp, Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp3 *exp_get_range (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_range (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_pt (Exp *v1, Exp *v2);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_pt (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_pt (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_sq (Exp *v1, Exp *v2);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_sq (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_sq (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_slice (Exp *v1, Exp *v2, Exp *v3);

/// Read an Exp of the indicate type. Returns a Tp3<Exp, Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp3 *exp_get_slice (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_slice (Exp *this);

/// Creates an expression of the indicated type.
/// 'arguments' is Arr<Exp>.
Exp *exp_pr (Exp *value, Arr *arguments);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Arr<Exp>>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_pr(Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_pr (Exp *this);

/// Creates an expression of the indicated type.
/// 'cases' is Arr<Tp<Arr<Exp>, Exp>>.
Exp *exp_switch (Exp *cond, Arr *cases);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Arr<Tp<Arr<Exp>, Exp>>>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_switch(Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_switch (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_not (Exp *value);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Exp *exp_get_not (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_not (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_minus (Exp *value);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Exp *exp_get_minus (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_minus (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_add (Exp *v1, Exp *v2);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_add (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_add (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_sub (Exp *v1, Exp *v2);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_sub (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_sub (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_mul (Exp *v1, Exp *v2);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_mul (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_mul (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_div (Exp *v1, Exp *v2);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_div (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_div (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_mod (Exp *v1, Exp *v2);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_mod (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_mod (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_and (Exp *v1, Exp *v2);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_and (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_and (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_or (Exp *v1, Exp *v2);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_or (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_or (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_greater (Exp *v1, Exp *v2);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_greater (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_greater (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_greater_eq (Exp *v1, Exp *v2);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_greater_eq (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_greater_eq (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_less (Exp *v1, Exp *v2);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_less (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_less (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_less_eq (Exp *v1, Exp *v2);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_less_eq (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_less_eq (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_eq (Exp *v1, Exp *v2);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_eq (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_eq (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_neq (Exp *v1, Exp *v2);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_neq (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_neq (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_ternary (Exp *v1, Exp *v2, Exp *v3);

/// Read an Exp of the indicate type. Returns a Tp3<Exp, Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp3 *exp_get_ternary (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_ternary (Exp *this);


// UTILITIES

/// Returns TRUE if 'this' is a binary operator.
int exp_is_binary (Exp *this);

/// Returns TRUE if 'this' is a string.
int exp_is_string (Exp *this);

/// Returns a string representation of 'this' type.
char *exp_type_to_str (Exp *this);

/// Returns a string representation of this.
char *exp_to_str (Exp *this);

/// Returns a JSON string representation of this.
char *exp_to_js (Exp *this);

#endif
