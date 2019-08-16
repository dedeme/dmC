// Copyright 06-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef IO_H
  #define IO_H

#include "dmc/std.h"
#include "data/Params.h"
#include "data/Path.h"

/// If directories of 'param' exist, returns an empty string. Otherwise returns
/// an error message.
char *io_dir_exists (Params *ps);

/// Arr[Path]. Returns every path to process.
Arr *io_all_files (Params *ps);

/// Arr[Char]. 'paths' is Arr[Path]. It filters files to scan with a lint.
Arr *io_filter_lint (Arr *paths, char *target);

/// 'files' is Arr[char]
void io_write_cache (Arr *files);

/// 'paths' is Arr[Path]
void io_copy_js (Arr *paths, char *target);

#endif
