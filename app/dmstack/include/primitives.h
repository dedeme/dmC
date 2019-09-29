// Copyright 28-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Primitives functions,

#ifndef PRIMITIVES_H
  #define PRIMITIVES_H

#include "dmc/async.h"
#include "Machine.h"
#include "Pmodule.h"
#include "Lib.h"

///
void primitives_init (void);

/// Returns Opt<pmodule_Fn>. Called at run time.
Opt *primitives_get (Symbol module, Symbol id);

///
void primitives_add_base (Heap *heap);

///
void primitives_add_lib (Lib *lib);

#endif
