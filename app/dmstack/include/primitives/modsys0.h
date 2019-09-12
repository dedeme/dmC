// Copyright 30-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Module of simple mathematical operations.

#ifndef MODULES_MODSYS0_H
  #define MODULES_MODSYS0_H

#include "dmc/async.h"
#include "Machine.h"

/// Operation +
void modsys0_add (Machine *m);

/// Operation -
void modsys0_sub (Machine *m);

/// Operation *
void modsys0_mul (Machine *m);

/// Operation /
void modsys0_div (Machine *m);

/// Operation %
void modsys0_mod (Machine *m);

/// Increment
void modsys0_incr (Machine *m);

/// Decrement
void modsys0_decr (Machine *m);

#endif
