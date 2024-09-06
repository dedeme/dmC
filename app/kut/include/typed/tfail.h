// Copyright 23-Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Fail management.

#ifndef TYPED_TFAIL_H
  #define TYPED_TFAIL_H

#include "stat.h"
#include "exp.h"
#include "ttype.h"

/// Returns a text type file:n: with the name of file and number of line.
char *tfail_position (StatCode *st);

/// Generic reader fail.
void tfail_throw (StatCode *st, char *msg_fmt, ...);

/// Check types and throws an EXC_KUT if fail.
/// 'n' is the number of expected (valid) types.
void tfail_check_type (StatCode *st, Ttype actual, int n, Ttype *expected);

/// Check type and throws an EXC_KUT if fail.
void tfail_check_type1 (StatCode *st, Ttype actual, Ttype expected);

/// Check expression type and throws an EXC_KUT if fail.
/// 'n' is the number of expected (valid) types.
void tfail_check_etype (
  StatCode *st, Exp *exp, Ttype actual, int n, Ttype *expected
);

/// Check expression type and throws an EXC_KUT if fail.
void tfail_check_etype1 (StatCode *st, Exp *exp, Ttype actual, Ttype expected);


#endif
