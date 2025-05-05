// Copyright 16-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Module reader.

#ifndef READERS_MODULE_RD_H
  #define READERS_MODULE_RD_H

#include "kut/rs.h"

/// Reads a file and returns its module (Rs<Module>).
///   is_main: TRUE if it is the main module.
///   fpath  : Canonical path of module.
///   id     : Compiler module identifier.
///   tx     : Code to read.
Rs *moduleRd_read (int is_main, char *fpath, char *id, char *tx);

#endif
