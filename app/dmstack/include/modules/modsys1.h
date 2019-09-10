// Copyright 31-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Logical operations.

#ifndef MODULES_MODSYS1_H
  #define MODULES_MODSYS1_H

#include "dmc/async.h"
#include "Machine.h"

/// Operation &&
void modsys1_and (Machine *m);

/// Operation ||
void modsys1_or (Machine *m);

/// Operation !
void modsys1_not (Machine *m);

/// Operation >
void modsys1_greater (Machine *m);

/// Operation >=
void modsys1_greater_eq (Machine *m);

/// Operation <
void modsys1_less (Machine *m);

/// Operation <=
void modsys1_less_eq (Machine *m);

#endif
