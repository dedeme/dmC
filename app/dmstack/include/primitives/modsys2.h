// Copyright 07-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Container operations.

#ifndef MODULES_MODSYS2_H
  #define MODULES_MODSYS2_H

#include "dmc/async.h"
#include "Machine.h"

/// Resturns container size.
void modsys2_size (Machine *m);

/// Return an element of container and removes the container.
void modsys2_get (Machine *m);

/// Return an element of container and keeps the container.
void modsys2_getplus (Machine *m);

/// Sets an element of container and removes the container.
/// Stack has: container - index - value.
void modsys2_set (Machine *m);

/// Sets an element of container and keeps the container.
/// Stack has: container - index - value.
void modsys2_setplus (Machine *m);

/// Returns a new subcontainer.
/// Stack has: container - start - end.
void modsys2_sub (Machine *m);

/// Returns a new subcontainer.
/// Stack has: container - cut. (container[cut] excluded).
void modsys2_left (Machine *m);

/// Returns a new subcontainer.
/// Stack has: container - cut. (container[cut] included).
void modsys2_right (Machine *m);

#endif
