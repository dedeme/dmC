// Copyright 11-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Momory modules

#ifndef MODULES_H
  #define MODULES_H

#include "dmc/async.h"

///
void modules_init (void);

/// Returns Opt<Map<Token>> - Functions from a module.
Opt *modules_module (char *module);

#endif
