// Copyright 16-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// BuyAndHold family

#ifndef BUYANDHOLD_H
  # define BUYANDHOLD_H

#include <dm/Json.h>
#include "Flea.h"

///
Flea *buyAndHold_new(Flea *f);

///
Flea *buyAndHold_restore(Flea *f, Json *serial);

#endif



