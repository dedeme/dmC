// Copyright 29-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Errors generator.

#ifndef FAILS_H
  #define FAILS_H

#include "dmc/std.h"
#include "Machine.h"

/// Error found an unexpected type.
///   m    : Machine
///   types: Expected type.
void fails_type (Machine *m, enum token_Type type);

/// Error found an unexpected type.
///   m    : Machine
///   n    : Number of errors.
///   types: array of expected types. For example:
///          (enum token_Type[]){token_INT, token_FLOAT}
void fails_types (Machine *m, int n, enum token_Type *types);

/// Error in list size.
void fails_size_list (Machine *m, Arr *list, int expected);

/// Error in field type of list.
void fails_type_list (Machine *m, Arr *list, int ix, enum token_Type expected);

#endif
