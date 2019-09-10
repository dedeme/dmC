// Copyright 29-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Errors generator.

#ifndef FAILS_H
  #define FAILS_H

#include "dmc/async.h"
#include "Machine.h"

///
void fails_init (void);

/// Register the machine of current thread.
void fails_register_machine (Machine *m);

/// Unregister the machine of current thread.
void fails_unregister_machine (void);

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

/// Error index out of range.
///   m  : Machine
///   min: Minimum value of range, inclusive.
///   max: Maximum value of range, inclusive.
///   ix : Value out of range.
void fails_range (Machine *m, int min, int max, int ix);

/// Throws a fails_range if ix < min or ix > max
///   m  : Machine
///   min: Minimum value of range, inclusive.
///   max: Maximum value of range, inclusive.
///   ix : Value out of range.
void fails_check_range (Machine *m, int min, int max, int ix);

#endif
