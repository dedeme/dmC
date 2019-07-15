// Copyright 15-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Makefile writer.

#ifndef WRITER_H
  #define WRITER_H

#include "dmc/std.h"

/// Generates Makefile file.
///   prg: Program name
///   libs: Arr[char] Libreries whom program depends on.
///   main_source: Entry point of compilation, without extension
///   os: Arr[char] .o files in OBJ_DIR, without extension.
///   partial: Final part of Makefile file.
void writer_mkmake (
  char *prg, Arr *libs, char *main_source, Arr *os, char *partial
);

#endif
