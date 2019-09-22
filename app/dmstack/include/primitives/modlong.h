// Copyright 20-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Long numbers module.

#ifndef PRIMITIVES_MODLONG_H
  #define PRIMITIVES_MODLONG_H

#include "dmc/async.h"
#include "primitives.h"

/// Returns Map<primitives_Fn>
Map *modlong_mk (void);

///
long modlong_to_long (Machine *m, Token *tk);

///
Token *modlong_from_long (long n);

#endif
