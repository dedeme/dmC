// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Built-in module str.

#ifndef MODS_MD_STR_H
  #define MODS_MD_STR_H

#include "bfunction.h"

/// Returns Bfunction with name 'fmane'.
/// Throw EXC_KUT if 'fname' does not exist.
Bfunction md_str_get (char *fname);

/// Returns an expression-string with the the byte at position 'i'.
Exp *md_str_at (char *s, int64_t i);

#endif
