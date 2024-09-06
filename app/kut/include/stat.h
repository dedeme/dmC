// Copyright 04-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Kut statments.

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

/// Returns an end statement.
Stat *stat_end (void);

/// Returns TRUE if 'this' is an end statement.
int stat_is_end (Stat *this);

/// Creates a statement of the indicated type.
/// Trows EXC_ILLEGAL_AGUMENT if arr_size(syms) < 2.
///   syms: Arr<Exp> with expressions of type EXP_SYM.
Stat *stat_indexed (Arr *syms);

/// Read a Stat of the indicate type. Returns Arr<Exp> (Exp of type EXP_SYM).
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_indexed (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_indexed (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_assign (Exp *left, Exp *right);

/// Read a Stat of the indicate type. Returns Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *stat_get_assign (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_assign (Stat *this);

/// Creates a statement of the indicated type.
///   new_sym: internal symbol to assign 'exp'. Necessary for the runner.
///   syms   : Arr<Exp> with expressions of type EXP_SYM or EXP_EMPTY.
///   exp    : Expresion with should resolve in EXP_ARR.
Stat *stat_arr_multi (Exp *new_sym, Arr *syms, Exp *exp);

/// Read a Stat of the indicate type. Returns Tp3<Exp, Arr, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp3 *stat_get_arr_multi (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_arr_multi (Stat *this);

/// Creates a statement of the indicated type.
///   new_sym: internal symbol to assign 'exp'. Necessary for the runner.
///   syms   : Arr<Exp> with expressions of type EXP_SYM or EXP_EMPTY.
///   exp    : Expresion with should resolve in EXP_DIC.
Stat *stat_dic_multi (Exp *new_sym, Arr *syms, Exp *exp);

/// Read a Stat of the indicate type. Returns Tp3<Exp, Arr, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp3 *stat_get_dic_multi (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_dic_multi (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_add_as (Exp *left, Exp *right);

/// Read a Stat of the indicate type. Returns Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *stat_get_add_as (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_add_as (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_sub_as (Exp *left, Exp *right);

/// Read a Stat of the indicate type. Returns Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *stat_get_sub_as (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_sub_as (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_mul_as (Exp *left, Exp *right);

/// Read a Stat of the indicate type. Returns Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *stat_get_mul_as (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_mul_as (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_div_as (Exp *left, Exp *right);

/// Read a Stat of the indicate type. Returns Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *stat_get_div_as (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_div_as (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_mod_as (Exp *left, Exp *right);

/// Read a Stat of the indicate type. Returns Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *stat_get_mod_as (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_mod_as (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_and_as (Exp *left, Exp *right);

/// Read a Stat of the indicate type. Returns Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *stat_get_and_as (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_and_as (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_or_as (Exp *left, Exp *right);

/// Read a Stat of the indicate type. Returns Tp<Exp, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *stat_get_or_as (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_or_as (Stat *this);

/// Creates a statement of the indicated type.
/// 'value' is 'exp_pr'.
Stat *stat_func (Exp *value);

/// Read a Stat of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Exp *stat_get_func (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_func (Stat *this);

/// Creates a statement of the indicated type.
/// 'stats' is an Arr<StatCode>.
Stat *stat_block (Arr *stats);

/// Read a Stat of the indicate type. Returns Arr<StatCode>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_block (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_block (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_block_close (void);

/// Returns TRUE if 'this' match the type.
int stat_is_block_close (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_break (void);

/// Returns TRUE if 'this' match the type.
int stat_is_break (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_continue (void);

/// Returns TRUE if 'this' match the type.
int stat_is_continue (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_trace (int is_complete, Exp *value);

/// Read a Stat of the indicate type. Returns Tp<int, Exp>.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Tp *stat_get_trace (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_trace (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_return (Exp *value);

/// Read a Stat of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Exp *stat_get_return (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_return (Stat *this);

/// Creates a statement of the indicated type.
/// 'finally' is Opt<StatCode>
Stat *stat_try (StatCode *try, int catch_var, StatCode *catch, Opt *finally);

/// Read a Stat of the indicate type. Returns an Arr[<StatCode>, <int>, <StatCode>, <Opt<StatCode>>]
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_try (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_try (Stat *this);

/// Creates a statement of the indicated type. 'cond' can be an empty expression.
Stat *stat_while (Exp *cond, StatCode *stat);

/// Read a Stat of the indicate type. Returns an Arr[Exp, <StatCode>]. Exp can
/// be an empty expression.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_while (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_while (Stat *this);

/// Creates a statement of the indicated type.
/// 'else_stat' is Opt<StatCode>.
Stat *stat_if(Exp *cond, StatCode *if_stat, Opt *else_stat);

/// Read a Stat of the indicate type. Returns an Arr[<Exp>, <StatCode>, Opt<StatCode>]
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_if (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_if (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_for (int var, Exp *collection, StatCode *stat);

/// Read a Stat of the indicate type. Returns an Arr[<int>, <Exp>, <StatCode>]
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_for (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_for (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_for_ix (int var_e, int var_ix,  Exp *collection, StatCode *stat);

/// Read a Stat of the indicate type. Returns an Arr[<int>, <int>, <Exp>, <StatCode>]
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_for_ix (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_for_ix (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_for_r (int var, Exp *start, Exp *end, StatCode *stat);

/// Read a Stat of the indicate type. Returns an Arr[<int>, <Exp>, <Exp>, <StatCode>]
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_for_r (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_for_r (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_for_rs (int var, Exp *start, Exp *end, Exp *step, StatCode *stat);

/// Read a Stat of the indicate type. Returns an Arr[<int>, <Exp>, <Exp>, <Exp>, <StatCode>]
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_for_rs (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_for_rs (Stat *this);

/// Creates a statement of the indicated type.
/// 'entries' is Arr<Tp<Arr<Exp>, StatCode>>
Stat *stat_switch (Exp *cond, Arr *entries);

/// Read a Stat of the indicate type. Returns an Arr[<Exp>, Arr<Tp<Arr<Exp>, StatCode>>]
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Arr *stat_get_switch (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_switch (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_import (char *module_path, int id);

/// Read a Stat of the indicate type. Returns an Arr[<char>, <int>].
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
/// Example
///   Arr *imp = stat_get_import(st);
///   char *mod_path = arr_get(imp, 0);
///   int sym = *(int *)arr_get(imp, 1);
Arr *stat_get_import (Stat *this);

/// Returns TRUE if 'this' match the type.
int stat_is_import (Stat *this);

/// Creates a statement of the indicated type.
Stat *stat_export (void);

/// Returns TRUE if 'this' match the type.
int stat_is_export (Stat *this);

///
char *stat_type_to_str (Stat *this);

///
char *stat_to_str (Stat *this);

#endif
