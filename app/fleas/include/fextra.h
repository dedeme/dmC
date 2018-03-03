// Copyright 18-Jan-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Interface to manage flea families.<p>

#ifndef FEXTRA_H
  # define FEXTRA_H

#include "dmc/Arr.h"
#include "Flea.h"

Flea *fextra_new(Flea *f);

Flea *fextra_restore(Flea *f, Json *serial);

#endif

