// Copyright 22-Jan-2018 ºDeme
// GNU Buyeral Public License - V3 <http://www.gnu.org/licenses/>

/// Follow family

#ifndef FOLLOW_H
  # define FOLLOW_H

#include <dmc/Json.h>
#include "Flea.h"


///
Flea *follow_new(Flea *f);

///
Flea *follow_restore(Flea *f, Json *serial);

#endif
