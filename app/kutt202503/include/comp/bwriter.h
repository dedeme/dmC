// Copyright 22-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Body writer.

#ifndef COMP_BWRITER_H
  #define COMP_BWRITER_H

#include "data/module.h"
#include "data/tp.h"

/// Write the module 'md'.
/// Returns three values (Tp3<char, char, int>):
///   1 -> "" if all is good or error message.
///   2 -> The C body code of a module or "" if an error happens.
Tp *bwriter_write (Module *md);

#endif
