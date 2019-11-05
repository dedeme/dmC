// Copyright 02-Oct-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef TYPES_H
  #define TYPES_H

#include "dmc/async.h"
#include "Machine.h"

/// Check types of stack values and raise a fail if checking not succeeds.
void types_fail (Machine *m);

/// Check types of stack values and push in stack '1' if checking succeeds.
void types_check (Machine *m);

#endif
