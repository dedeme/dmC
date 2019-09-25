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

/// Sets an element of container using its value and removes the container.
/// Stack has: container - index - function.
void modglobal2_up (Machine *m);

/// Sets an element of container using its value and keeps the container.
/// Stack has: container - index - function.
void modglobal2_upplus (Machine *m);

/// Return its element from a reference and removes the reference.
void modglobal2_ref_get (Machine *m);

/// Sets a element in a reference and removes the reference.
/// Stack has: container - value.
void modglobal2_ref_set (Machine *m);

/// Sets a element in a reference and keeps the reference.
/// Stack has: container - value.
void modglobal2_ref_setplus (Machine *m);

/// Sets a element in a reference using its value and removes the reference.
/// Stack has: container - function.
void modglobal2_ref_up (Machine *m);

/// Sets a element in a reference using its value and keeps the reference.
/// Stack has: container - function.
void modglobal2_ref_upplus (Machine *m);

#endif
