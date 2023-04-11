// Copyright 08-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Built-in module tcp.

#ifndef MODS_MD_TCP_H
  #define MODS_MD_TCP_H

#include "bfunction.h"

/// Returns Bfunction with name 'fmane'.
/// Throw EXC_KUT if 'fname' does not exist.
Bfunction md_tcp_get (char *fname);

#endif
