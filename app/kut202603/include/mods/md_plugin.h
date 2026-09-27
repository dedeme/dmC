// Copyright 03-Feb-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Built-in module plugin.

#ifndef MODS_MD_PLUGIN_H
  #define MODS_MD_PLUGIN_H

#include "bfunction.h"

/// Returns Bfunction with name 'fmane'.
/// Throw EXC_KUT if 'fname' does not exist.
Bfunction md_plugin_get (char *fname);

#endif
