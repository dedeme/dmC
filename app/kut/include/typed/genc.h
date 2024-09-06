// Copyright 22-Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// C code generator.

#ifndef TYPED_GEN_C_H
  #define TYPED_GEN_C_H

#include "tfunction.h"

/// 'path' is the path of file '.so' (without .so)
void genc_init (char *path);

/// Generates C code.
void genc_write (Tfunction *fn);

/// Compile C code.
void genc_compile (void);

/// Load .so library if created.
void genc_load_so (void);

/// Execute C function without return.
void genc_run_procedure (char *id, Tval *values);

/// Execute C function with return.
Tval genc_run_function (char *id, Tval *values);

#endif
