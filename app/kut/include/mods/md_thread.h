// Copyright 07-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Built-in module thread.

#ifndef MODS_MD_THREAD_H
  #define MODS_MD_THREAD_H

#include "bfunction.h"

/// Returns Bfunction with name 'fmane'.
/// Throw EXC_KUT if 'fname' does not exist.
Bfunction md_thread_get (char *fname);

#endif
