// Copyright 28-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Primitives functions,

#ifndef PRIMITIVES_H
  #define PRIMITIVES_H

#include "dmc/async.h"
#include "Machine.h"

///
typedef void (*primitives_Fn) (Machine *m);

///
void primitives_init (void);

/// Returns Opt<primitives_Fn>
Opt *primitives_get (char *module, char *id);

/// Returns Opt<Map<Token>> - Functions from a module.
Opt *primitives_module (char *module);

#endif
