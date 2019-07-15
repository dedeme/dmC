// Copyright 14-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef READER_H
  #define READER_H

#include "dmc/std.h"

/// If checking fails, shows a message and returns 1.
int reader_check_directories (void);

/// If checking fails, shows a message and returns 1.
///   libs: Arr[char] Static libraries name.
int reader_check_libraries (Arr *libs);

/// Returns dependencies of 'file' (searches in src/<file>.c and
/// include/<file>.h)
///   file: Path to search without extension.
///   return: Arr[char] Dependencies path without extension
Arr *reader_dependencies (char *file);

#endif
