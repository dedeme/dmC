// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Built-in modules management.

#ifndef BMODULE_H
  #define BMODULE_H

#include "bfunction.h"

/// Returns TRUE if 'md_name' is a valid module name.
int bmodule_exists (char *md_name);

/// Returns the function 'fn' of module 'md'.
/// Throws EXC_KUT if 'fn' does not exist.
Bfunction bmodule_get_function (char *md, char *fn);

#endif
