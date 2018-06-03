// Copyright 16-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Class parser.

#ifndef PARSER_PCLASS_H
  # define PARSER_PCLASS_H

#include <dmc/all.h>
#include "ast/Class.h"

/// pclass_main checks main class. It checks the same as pclass()
/// Throws tx_exception
void pclass_main(Class *c, Atype *generics);

/// pclass checks a normal class and resolves attributes type. It checks:
///   * Local parameter
///   * Inheritance is not cyclic and using correct generics
///   * Attributes type
/// Throws tx_exception
void pclass(Class *caller, Class *c, Atype *generics);

/// pclass_implementation checks if implementations are valid.
/// It should be called after add 'c' to Program.
/// Throws tx_exception
void pclass_implementation(Class *c, Atype *generics);

#endif


