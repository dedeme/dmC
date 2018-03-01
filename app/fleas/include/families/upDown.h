// Copyright 16-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Up-Down family

#ifndef UP_DOWN_H
  # define UP_DOWN_H

#include <dm/Json.h>
#include "Flea.h"

///
Flea *updown_new(Flea *f);

///
Flea *updown_restore(Flea *f, Json *serial);

#endif



