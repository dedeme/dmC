// Copyright 16-Apr-2023 ºDeme
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

/// Returns the javascript code of 'this'.
char *exp_get_js (Exp *this);

/// Sets the javascript code of 'this'.
void exp_set_js (Exp *this, char *js);

/// Returns the javascript code of 'this' inserting 'ins' after the last blank.
char *exp_js_insert (Exp *this, char *ins);

/// Returns blanks of the javascript code of 'this' appending 'repl'.
char *exp_js_replace (Exp *this, char *repl);

/// Creates an expression of the indicated type.
Exp *exp_null (Exp *value, char *js);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Exp *exp_get_null (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_null (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_class (char *value, char *js);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
char *exp_get_class (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_class (Exp *this);

/// Creates an expression of the indicated type. 'value' is Arr<Exp>.
Exp *exp_new (char *id, Arr *value, char *js);

/// Read an Exp of the indicate type. Returns Tp<char, Arr<Exp>>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_new (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_new (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_eval (char *value, char *js);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
char *exp_get_eval (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_eval (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_bool (int value, char *js);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
int exp_get_bool (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_bool (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_int (int64_t value, char *js);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
int64_t exp_get_int (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_int (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_float (double value, char *js);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
double exp_get_float (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_float (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_string (char *value, char *js);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
char *exp_get_string (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_string (Exp *this);

/// Creates an expression of the indicated type. 'value' is type Arr<Exp>.
Exp *exp_array (Arr *value, char *js);

/// Read an Exp of the indicate type. Returns an array of type Arr<Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *exp_get_array (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_array (Exp *this);

/// Creates an expression of the indicated type. 'value' is type Map<Exp>.
Exp *exp_map (Map *value, char *js);

/// Read an Exp of the indicate type. Returns an array of type Map<Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Map *exp_get_map (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_map (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_function (Function *value, char *js);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Function *exp_get_function (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_function (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_sym (char *value, char *js);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
char *exp_get_sym (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_sym (Exp *this);

/// Creates an expression of the indicated type. e3 is Opt<Exp>
Exp *exp_range (Exp *v1, Exp *v2, Opt *v3, char *js);

/// Read an Exp of the indicate type. Returns a Tp3<Exp, Exp, Opt<Exp>>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp3 *exp_get_range (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_range (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_pt (Exp *v1, Exp *v2, char *js);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_pt (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_pt (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_sq (Exp *v1, Exp *v2, char *js);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_sq (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_sq (Exp *this);

/// Creates an expression of the indicated type. v2 and v3 are Opt<Exp>.
Exp *exp_slice (Exp *v1, Opt *v2, Opt *v3, char *js);

/// Read an Exp of the indicate type. Returns a Tp3<Exp, Opt<Exp>, Opt<Exp>>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp3 *exp_get_slice (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_slice (Exp *this);

/// Creates an expression of the indicated type.
/// 'arguments' is Arr<Exp>.
Exp *exp_pr (Exp *value, Arr *arguments, char *js);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Arr<Exp>>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_pr(Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_pr (Exp *this);

/// Creates an expression of the indicated type.
/// 'cases' is Arr<Tp<Exp, Exp>>.
Exp *exp_switch (Exp *cond, Arr *cases, char *js);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Arr<Tp<Exp, Exp>>>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_switch(Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_switch (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_not (Exp *value, char *js);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Exp *exp_get_not (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_not (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_minus (Exp *value, char *js);

/// Read an Exp of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Exp *exp_get_minus (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_minus (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_add (Exp *v1, Exp *v2, char *js);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_add (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_add (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_sub (Exp *v1, Exp *v2, char *js);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_sub (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_sub (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_mul (Exp *v1, Exp *v2, char *js);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_mul (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_mul (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_div (Exp *v1, Exp *v2, char *js);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_div (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_div (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_mod (Exp *v1, Exp *v2, char *js);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_mod (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_mod (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_and (Exp *v1, Exp *v2, char *js);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_and (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_and (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_or (Exp *v1, Exp *v2, char *js);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_or (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_or (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_greater (Exp *v1, Exp *v2, char *js);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_greater (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_greater (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_greater_eq (Exp *v1, Exp *v2, char *js);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_greater_eq (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_greater_eq (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_less (Exp *v1, Exp *v2, char *js);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_less (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_less (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_less_eq (Exp *v1, Exp *v2, char *js);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_less_eq (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_less_eq (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_eq (Exp *v1, Exp *v2, char *js);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_eq (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_eq (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_neq (Exp *v1, Exp *v2, char *js);

/// Read an Exp of the indicate type. Returns a Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *exp_get_neq (Exp *this);

/// Returns TRUE if 'this' match the type.
int exp_is_neq (Exp *this);

/// Creates an expression of the indicated type.
Exp *exp_ternary (Exp *v1, Exp *v2, Exp *v3, char *js);

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

#endif
