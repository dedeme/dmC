// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Immutable classses of Kut statments.

#ifndef STAT_H
  #define STAT_H

#include "exp.h"

/// Statement structure.
typedef struct stat_Stat Stat;

/// Statement structure placed in code.
typedef struct stat_StatCode StatCode;

/// Constructor.
StatCode *stat_code_new(int file_ix, int nline, Stat *stat);

/// Returns the file index value.
int stat_code_file_ix(StatCode *this);

/// Returns the file line value.
int stat_code_line(StatCode *this);

/// Returns the enveloped statement.
Stat *stat_code_stat(StatCode *this);

/// Returns the javascript code of 'this'.
char *stat_get_js (Stat *this);

/// Sets the javascript code of 'this'.
void stat_set_js (Stat *this, char *js);

/// Returns an end statement.
Stat *stat_end (void);

/// Returns TRUE if 'this' is an end statement.
int stat_is_end (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_assign (Exp *left, Exp *right, char *js);

/// Read a Stat of the indicate type. Returns Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *stat_get_assign (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_assign (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_do (char *js_inject, char *js);

/// Read a Stat of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
char *stat_get_do (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_do (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_await (Exp *fn_call, char *js);

/// Read a Stat of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Exp *stat_get_await (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_await (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_add_as (Exp *left, Exp *right, char *js);

/// Read a Stat of the indicate type. Returns Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *stat_get_add_as (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_add_as (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_sub_as (Exp *left, Exp *right, char *js);

/// Read a Stat of the indicate type. Returns Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *stat_get_sub_as (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_sub_as (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_mul_as (Exp *left, Exp *right, char *js);

/// Read a Stat of the indicate type. Returns Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *stat_get_mul_as (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_mul_as (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_div_as (Exp *left, Exp *right, char *js);

/// Read a Stat of the indicate type. Returns Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *stat_get_div_as (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_div_as (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_mod_as (Exp *left, Exp *right, char *js);

/// Read a Stat of the indicate type. Returns Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *stat_get_mod_as (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_mod_as (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_and_as (Exp *left, Exp *right, char *js);

/// Read a Stat of the indicate type. Returns Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *stat_get_and_as (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_and_as (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_or_as (Exp *left, Exp *right, char *js);

/// Read a Stat of the indicate type. Returns Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *stat_get_or_as (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_or_as (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_func (Exp *value, char *js);

/// Read a Stat of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Exp *stat_get_func (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_func (Stat *this);

/// Creates a statement of the indicated type.
/// 'stats' is an Arr<StatCode>.
Stat *stat_block (Arr *stats, char *js);

/// Read a Stat of the indicate type. Returns Arr<StatCode>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_block (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_block (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_block_close (char *js);

/// Returns TRUE if 'this' match the type.
int stat_is_block_close (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_break (char *js);

/// Returns TRUE if 'this' match the type.
int stat_is_break (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_continue (char *js);

/// Returns TRUE if 'this' match the type.
int stat_is_continue (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_trace (Exp *value, char *js);

/// Read a Stat of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Exp *stat_get_trace (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_trace (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_throw (Exp *value, char *js);

/// Read a Stat of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Exp *stat_get_throw (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_throw (Stat *this);

/// Creates a statement of the indicated type. 'value' is Opt<Exp>
Stat *stat_return (Opt *value, char *js);

/// Read a Stat of the indicate type. Returns Opt<Exp>
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Opt *stat_get_return (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_return (Stat *this);

/// Creates a statement of the indicated type.
/// 'finally' is Opt<StatCode>
Stat *stat_try (
  StatCode *try, char *catch_var, StatCode *catch, Opt *finally, char *js
);

/// Read a Stat of the indicate type. Returns an Arr[<StatCode>, <char>, <StatCode>, <Opt<StatCode>>]
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_try (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_try (Stat *this);

/// Creates a statement of the indicated type. 'cond' is Opt<Exp>
Stat *stat_while (Opt *cond, StatCode *stat, char *js);

/// Read a Stat of the indicate type. Returns an Arr[Opt<Exp>, <StatCode>].
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_while (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_while (Stat *this);

/// Creates a statement of the indicated type.
/// 'else_stat' is Opt<StatCode>.
Stat *stat_if(Exp *cond, StatCode *if_stat, Opt *else_stat, char *js);

/// Read a Stat of the indicate type. Returns an Arr[<Exp>, <StatCode>, Opt<StatCode>]
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_if (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_if (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_for (char *var, Exp *collection, StatCode *stat, char *js);

/// Read a Stat of the indicate type. Returns an Arr[<char>, <Exp>, <StatCode>]
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_for (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_for (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_for_ix (
  char *var_e, char *var_ix,  Exp *collection, StatCode *stat, char *js
);

/// Read a Stat of the indicate type. Returns an Arr[<char>, <char>, <Exp>, <StatCode>]
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_for_ix (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_for_ix (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_for_r (char *var, Exp *start, Exp *end, StatCode *stat, char *js);

/// Read a Stat of the indicate type. Returns an Arr[<char>, <Exp>, <Exp>, <StatCode>]
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_for_r (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_for_r (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_for_rs (
  char *var, Exp *start, Exp *end, Exp *step, StatCode *stat, char *js
);

/// Read a Stat of the indicate type. Returns an Arr[<char>, <Exp>, <Exp>, <Exp>, <StatCode>]
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_for_rs (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_for_rs (Stat *this);

/// Creates a statement of the indicated type.
/// 'entries' is Arr<Tp<Exp, StatCode>>
Stat *stat_switch (Exp *cond, Arr *entries, char *js);

/// Read a Stat of the indicate type. Returns an Arr[<Exp>, Arr<Tp<Exp, StatCode>>]
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_switch (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_switch (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_import (char *module_path, char *id, char *js);

/// Read a Stat of the indicate type. Returns an Arr[<char>, <char>].
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_import (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_import (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_export (char *js);

/// Returns TRUE if 'this' match the type.
int stat_is_export (Stat *this);

///
char *stat_type_to_str (Stat *this);

///
char *stat_to_str (Stat *this);

#endif
