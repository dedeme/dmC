// Copyright 12-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Expression data.
/// NOTE: There is stat_error (+ message).

#ifndef DATA_STAT_H
  #define DATA_STAT_H

#include "data/exp.h"

/// Token types
typedef enum {
  stat_error,
  stat_indexed, stat_assign, stat_multiassign,
  stat_addas, stat_subas, stat_mulas, stat_divas, stat_modas,
  stat_andas, stat_oras,
  stat_function, stat_block, stat_close_block,
  stat_break, stat_continue, stat_return, stat_throw,
  stat_trace,
  stat_while, stat_if, stat_switch,
  stat_for, stat_for_range,
  stat_try,
  stat_import
} StatT;

/// Record structure.
struct stat_Stat {
  int ln; // Number of line.
  StatT tp; // Statement type.
  void *value; // Statement value. It depends on tp.
};

typedef struct stat_Stat Stat;

/// Statement constuctor
///   ln : Line number.
///   msg: Error message.
Stat *stat_new_error (int ln, char *msg);

/// Statement constuctor for indexed and enum.
///   ln  : Line number.
///   tp  : Type (tuple for indexed, type_bool for enum)
///   fn  : Function name for indexed, "" for enum.
///   pars: (Arr<char>) Parameters. Some elements can be "" for enum.
Stat *stat_new_indexed (int ln, Type *type, char *fn, Arr *pars);

/// Statement constuctor
///   ln   : Line number.
///   symSq: Expression of type symbol or square.
///   value: Expression to assign.
Stat *stat_new_assign (int ln, Exp *symSq, Exp *value);

/// Statement constuctor
///   ln   : Line number.
///   syms : (Arr<char>) No empty sumbols list. Elements not assigned
///          have "" value.
///          Every symbol of 'syms' must not be "".
///   value: Tuple expression.
Stat *stat_new_multiassign (int ln, Arr *syms, Exp *value);

/// Statement constuctor
///   ln   : Line number.
///   var  : Expression type square.
///   value: Expression to assign.
Stat *stat_new_addas (int ln, Exp *var, Exp *value);

/// Statement constuctor
///   ln   : Line number.
///   var  : Expression type square.
///   value: Expression to assign.
Stat *stat_new_subas (int ln, Exp *var, Exp *value);

/// Statement constuctor
///   ln   : Line number.
///   var  : Expression type square.
///   value: Expression to assign.
Stat *stat_new_mulas (int ln, Exp *var, Exp *value);

/// Statement constuctor
///   ln   : Line number.
///   var  : Expression type square.
///   value: Expression to assign.
Stat *stat_new_divas (int ln, Exp *var, Exp *value);

/// Statement constuctor
///   ln   : Line number.
///   var  : Expression type square.
///   value: Expression to assign.
Stat *stat_new_modas (int ln, Exp *var, Exp *value);

/// Statement constuctor
///   ln   : Line number.
///   var  : Expression type square.
///   value: Expression to assign.
Stat *stat_new_andas (int ln, Exp *var, Exp *value);

/// Statement constuctor
///   ln   : Line number.
///   var  : Expression type square.
///   value: Expression to assign.
Stat *stat_new_oras (int ln, Exp *var, Exp *value);

/// Statement constuctor.
///   ln: Line number.
///   fn: Parenthesis expression.
Stat *stat_new_function (int ln, Exp *fn);

/// Statement constuctor
///   ln   : Line number.
///   stats: (Arr<Stat>) Block statements.
Stat *stat_new_block (int ln, Arr *sts);

/// Statement constuctor
///   ln: Line number.
Stat *stat_new_close_block (int ln);

/// Statement constuctor
///   ln: Line number.
Stat *stat_new_break (int ln);

/// Statement constuctor
///   ln: Line number.
Stat *stat_new_continue (int ln);

/// Statement constuctor
///   ln   : Line number.
///   value: (Opt<Exp>) Expression to return.
Stat *stat_new_return  (int ln, Opt *value);

/// Statement constuctor.
///   ln   : Line number.
///   value: Error message.
Stat *stat_new_throw (int ln, Exp *value);

/// Statement constuctor.
///   ln         : Line number.
///   is_complete: 'true' if 'value' is shown completely.
///   value      : Message to show.
Stat *stat_new_trace (int ln, int is_complete, Exp *value);
/// Statement constuctor.
///   ln  : Line number.
///   cond: (Opt<Exp>) Condition.
///   st  : Statement of type 'block'.
Stat *stat_new_while (int ln, Opt* cond, Stat *st);

/// Statement constuctor.
///   ln      : Line number.
///   cond    : Condition.
///   st_true : Statement of type 'block'.
///   st_false: (Opt<Stat>) Statement of type 'block'
Stat *stat_new_if (int ln, Exp *cond, Stat *st_true, Opt *st_false);

/// Statement constuctor.
///   ln     : Line number.
///   val    : Value to check.
///   entries: (Arr<Tp<Arr<Exp>, Stat>>) 'switch' entries.
///            The statement of entries is a 'block'.
Stat *stat_new_switch (int ln, Exp *val, Arr* entries);

/// Statement constuctor.
///   ln : Line number.
///   ix : (Opt<char>) Variable for index or key.
///   var: Variable for values.
///   ct : Container.
///   st : Statement of type 'block'.
Stat *stat_new_for (int ln, Opt *ix, char *var, Exp *ct, Stat *st);

/// Statement constuctor.
///   ln : Line number.
///   var: Variable.
///   v1 : Start value.
///   v2 : End value.
///   v3 : (Opt<Exp>) Step value.
///   st : Statement of type 'block'.
Stat *stat_new_for_range (
  int ln, char *var, Exp *v1, Exp *v2, Opt *v3, Stat *st
);

/// Statement constuctor.
///   ln        : Line number.
///   st        : Statement of type 'block'.
///   var       : Catch variable.
///   st_catch  : Statement of type 'block'.
Stat *stat_new_try (
  int ln, Stat *st, char *var, Stat *st_catch
);

/// Statement constuctor.
///   ln        : Line number.
///   path: Module path.
///   name: (Opt<char>) Local name.
Stat *stat_new_import (int ln, char *path, Opt *name);

///
char *stat_to_str (Stat *this);

#endif

