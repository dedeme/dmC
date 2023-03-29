// Copyright 07-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Code reader.

#ifndef READER_H
  #define READER_H

#include "reader/cdr/cdr.h"
#include "module.h"

/// Read the 'main' module.
/// Throw EXC_KUT if the module can not be read.
Module *reader_read_main_block (Cdr *cdr);

/// Read a code block. Returns Arr<StatCode>.
/// Throw EXC_KUT if the reading fails.
Arr *reader_read_block (Cdr *cdr);

#endif
