// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Runner fails management.

#ifndef RUNNER_FAIL_H
  #define RUNNER_FAIL_H

#include "exp.h"
#include "kut/arr.h"

/// Adds 'stack trace' to message.
///   msg  : Main message.
///   stack: Arr<StatCode>. Stack trace.
char *fail_add_stack (char *msg, Arr *stack);

/// Returns a message for type error.
///   expected: Expected type (e.g. "int" or "int, string")
///   exp     : Actual expression.
char *fail_type (char *expected, Exp *exp);

/// Returns a message 'bfunction not found'.
///   mod : Bmodule.
///   func: Bfunction.
char *fail_bfunction (char *mod, char *func);

#endif
