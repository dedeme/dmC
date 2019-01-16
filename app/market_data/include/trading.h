// Copyright 15-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Current trading position.

#ifndef TRADING_H
  #define TRADING_H

#include "dmc/std.h"
#include "dmc/Darr.h"

/// Returns the current trading position (with strip) of each nick. Values are
/// in the same order as NICKS (defined in DEFS.h)<br>
/// Values less than 0 are supports, greater than 0 are resistences and 0 are
/// not operation.
Darr *trading_read_new();

#endif
