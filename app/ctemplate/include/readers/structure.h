// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Structure reader.<br>
/// Definition:
///   HEAD
///   ··BODY

#ifndef READERS_STRUCTURE_H
  #define READERS_STRUCTURE_H


#include "data.h"
#include "RW.h"

/// Reads an array of Structure in sts;
Structure *structure_read(RW *rw);

#endif

