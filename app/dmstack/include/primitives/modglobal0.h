// Copyright 30-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Module of simple mathematical operations.

#ifndef MODULES_MODGLOBAL0_H
  #define MODULES_MODGLOBAL0_H

#include "dmc/async.h"
#include "Machine.h"

/// Operation +
void modglobal0_add (Machine *m);

/// Operation -
void modglobal0_sub (Machine *m);

/// Operation *
void modglobal0_mul (Machine *m);

/// Operation /
void modglobal0_div (Machine *m);

/// Operation %
void modglobal0_mod (Machine *m);

/// Increment
void modglobal0_incr (Machine *m);

/// Decrement
void modglobal0_decr (Machine *m);

#endif
