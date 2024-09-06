// Copyright 27-Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Selector of typed built-in modules.

#ifndef TYPED_TBMODULES_H
  #define TYPED_TBMODULES_H

#include "ttype.h"

/// Data returned by 'tbmodules_get'.
typedef struct tbmodules_TbmodRs {
  char *code; // C function code or error if 'tret' is ttype_ERROR
  Ttype tret;
}TbmodRs;

/// Returns built-in function data.
///   module: Typed module identifier as writen in Kut code.
///   function: Function identifier as write in Kut code.
///   as_expression: TRUE if function is as expression in code.
///   type: Function type, using an empty string when there is no return.
///         (e.g. "ii|" instead of "ii|0").
///   pars: C function arguments (e.g "my_array,ix").
TbmodRs *tbmodules_get (
  char *module, char *function, int as_expression, char *type, char *pars
);

#endif
