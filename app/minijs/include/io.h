// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// File system management.

#ifndef IO_H
  # define IO_H

#include "types/Either.h"
#include "dmc/ct/Lchar.h"
#include "dmc/ct/Ojson.h"
#include "types/Cpath.h"

/// Returns the Cpath of 'relative_path' or fail if 'relative_path' can no be
/// found or is found duplicate.
Either/*Cpath*/ *io_cpath(Lchar *roots, char *relative_path);

/// Reads a class from cache
Ojson *io_read_class(Cpath *path);

#endif

