// Copyright 04-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Built-in module cryp.

#ifndef MODS_MD_CRYP_H
  #define MODS_MD_CRYP_H

#include "bfunction.h"

/// Returns Bfunction with name 'fmane'.
/// Throw EXC_KUT if 'fname' does not exist.
Bfunction md_cryp_get (char *fname);

#endif
