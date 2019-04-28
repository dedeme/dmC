// Copyright 15-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Current trading position.

#ifndef TRADING_H
  #define TRADING_H

#include "dmc/std.h"
#include "dmc/Darr.h"

/// Returns:
///   last_qs_new: The last close of each nick.
///   signals_new: The current trading position (with strip) of each nick.
///                Values less than 0 are supports, greater than 0 are
///                resistences and 0 are not operation.
///   nicks:       Arr[char]. Nicks to read
void trading_read_new(Darr **last_qs_new, Darr **signals_new, Arr *nicks);

#endif
