// Copyright 02-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Tkut code reader.

#ifndef READER_H
  #define READER_H

#include "read/module.h"
#include "run/rn_code.h"

/// Read the main file.
RnCode *reader_read_main_file (char *module_name);

/// Read a file.
Module *reader_read_file (char *module_name);

/// Read a string.
RnCode *reader_read (char *kt_code);

#endif
