// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Strucuture head reader.<br>
/// Definition:
///   [-|+]struct: [-]Id

#ifndef READERS_HEAD_H
  #define READERS_HEAD_H

#include "data.h"
#include "RW.h"

/// Returns a Structure head.
Head *head_read(RW *rw);

#endif

