// Copyright 12-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Expression data.
/// NOTE: There are exp_error (+ message).

#ifndef DATA_EXP_H
  #define DATA_EXP_H

#include "kut/arr.h"
#include "kut/opt.h"
#include "data/type.h"

typedef struct stat_Stat Stat;

/// Token types
typedef enum {
  exp_error, exp_boolean, exp_integer, exp_float, exp_string,
  exp_array, exp_array0, exp_dictionary, exp_dictionary0,
  exp_tuple, exp_function, exp_symbol, exp_range,
  exp_point, exp_square, exp_slice, exp_parenthesis,
  exp_group, exp_unary, exp_binary, exp_ternary, exp_switch
} ExpT;

/// Record structure.
typedef struct {
  int ln; // Number of line.
  ExpT tp; // Expression type.
  void *value; // Expression value. It depends on tp.
} Exp;

/// Expression constuctor
///   ln : Line number.
///   msg: Error message.
Exp *exp_new_error (int ln, char *msg);

/// Expression constuctor
///   ln: Line number.
///   v : "true" or "false".
Exp *exp_new_boolean (int ln, char *v);

/// Expression constuctor
///   ln: Line number.
///   v : An integer.
Exp *exp_new_integer (int ln, char *v);

/// Expression constuctor
///   ln: Line number.
///   v : A float.
Exp *exp_new_float (int ln, char *v);

/// Expression constuctor
///   ln: Line number.
///   v : A String.
Exp *exp_new_string (int ln, char *v);

/// Expression constuctor
///   ln: Line number.
///   v : (Arr<Exp>) Non empty array.
Exp *exp_new_array (int ln, Arr *v);

/// Expression constuctor
///   ln: Line number.
///   tp: (Opt<Type>) Element type or empty if it was not defined.
Exp *exp_new_array0 (int ln, Opt *tp);

/// Expression constuctor
///   ln: Line number.
///   v : (Arr<Tp<Exp, Exp>>) Pairs [[symbolOrString, value].].
Exp *exp_new_dictionary (int ln, Arr *v);

/// Expression constuctor
///   ln : Line number.
///   tp: (Opt<Type>) Element type or empty if it was not defined.
Exp *exp_new_dictionary0 (int ln, Opt *tp);

/// Expression constuctor
///   ln: Line number.
///   v : (Arr<Exp>) Non empty array.
Exp *exp_new_tuple (int ln, Arr *v);

/// Function constuctor
/// Correction of type an match number parameteres checked.
///   ln  : Line number.
///   tp  : Function type.
///   pars: (Arr<char>) Parameters list.
///   code: Block statement with the function code.
Exp *exp_new_function (int ln, Type *tp, Arr *pars, Stat *code);

/// Expression constuctor
///   ln: Line number.
///   v : A String.
Exp *exp_new_symbol (int ln, char *v);

/// Expression constuctor
///   ln: Line number.
///   v : (Arr<Exp>) Array with numeric expressions (1, 2 or 3 integers).
Exp *exp_new_range (int ln, Arr *v);

/// Expression constuctor
///   ln: Line number.
///   md: Module identifier.
///   fn: Function name.
Exp *exp_new_point (int ln, char *md, char *fn);

/// Expression constuctor
///   ln: Line number.
///   ct: Container.
///   ix: Index.
Exp *exp_new_square (int ln, Exp *ct, Exp *ix);

/// Expression constuctor
///   ln   : Line number.
///   ct   : Container.
///   start: (Opt<Exp>) Start index.
///   end  : (Opt<Exp>) End index.
Exp *exp_new_slice (int ln, Exp *ct, Opt *start, Opt *end);

/// Expression constuctor
///   ln  : Line number.
///   fn  : Function.
///   pars: (Arr<Exp>)Parameters.
Exp *exp_new_parenthesis (int ln, Exp *fn, Arr *pars);

/// Expression constuctor
///   ln : Line number.
///   v  : Expression between parentheses.
Exp *exp_new_group (int ln, Exp *v);

/// Expression constuctor
///   ln : Line number.
///   sym: Unary function symbol (! or -).
///   v  : Expression to apply the function.
Exp *exp_new_unary (int ln, char *sym, Exp *v);

/// Expression constuctor
///   ln : Line number.
///   sym: Binary function symbol.
///   v1 : Left expression to apply the function.
///   v2 : Right expression to apply the function.
Exp *exp_new_binary (int ln, char *sym, Exp *v1, Exp *v2);

/// Expression constuctor
///   ln : Line number.
///   v1 : Condition expression to apply the function.
///   v2 : 'true' expression.
///   v3 : 'false' expression.
Exp *exp_new_ternary  (int ln, Exp *v1, Exp *v2, Exp *v3);

/// Expression constuctor
///   ln : Line number.
///   ex : Expression to compare.
///   es : (Arr<Tp<Arr<Exp>, Exp>>) Switch entries.
Exp *exp_new_switch (int ln, Exp *ex, Arr *es);

/// Returns a representation of 'this' type.
char *exp_type_to_str (Exp *this);

/// Returns a representation of 'this'.
char *exp_to_str (Exp *this);

#endif

