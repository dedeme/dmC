// Copyright 17-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Follow with Moving Average family

#ifndef FOLLOWMA_H
  # define FOLLOWMA_H

#include <dm/Json.h>
#include "Flea.h"

///
Flea *followma_new(Flea *f);

///
Flea *followma_restore(Flea *f, Json *serial);

#endif
