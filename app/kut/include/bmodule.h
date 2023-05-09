// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Built-in modules management.

#ifndef BMODULE_H
  #define BMODULE_H

#include "bfunction.h"

/// Returns TRUE if 'md_name' is a valid module symbol.
int bmodule_exists (int md_sym);

/// Returns the function 'fn_sym' of module 'md_sym'.
/// Throws EXC_KUT if 'fn' does not exist.
Bfunction bmodule_get_function (int md_sym, int fn_sym);

#endif
