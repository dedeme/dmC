// Copyright 25-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef WRITER_H
  #define WRITER_H

#include "dmc/std.h"

/// Writes temporary .h and .c files
///   finclude: Orginal .h file
///   fsrc: Original .c file
///   tps: Is Arr[Tpl]
///   return: 0 if an error has happenned.
int writer_mk_tmp(char *finclude, char *fsrc, Arr *tps);

/// Overwrite temporary on original .h and .c files
///   finclude: Orginal .h file
///   fsrc: Original .c file
void writer_copy_tmp(char *finclude, char *fsrc);

#endif
