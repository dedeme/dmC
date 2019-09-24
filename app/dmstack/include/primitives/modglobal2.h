// Copyright 07-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Container operations.

#ifndef MODULES_MODGLOBAL2_H
  #define MODULES_MODGLOBAL2_H

#include "dmc/async.h"
#include "Machine.h"

/// Resturns container size.
void modglobal2_size (Machine *m);

/// Return an element of container and removes the container.
void modglobal2_get (Machine *m);

/// Sets an element of container and removes the container.
/// Stack has: container - index - value.
void modglobal2_set (Machine *m);

/// Sets an element of container and keeps the container.
/// Stack has: container - index - value.
void modglobal2_setplus (Machine *m);

#endif
