// Copyright 17-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Reader of nicks and quotes.

#ifndef IO_H
  #define IO_H

#include "dmc/std.h"
#include "dmc/Darr.h"

/// Returns Arr[char] with selected nicks
Arr *io_nicks (void);

/// Returns daily quotes of a nick
Darr *io_quotes (char *nick);

#endif
