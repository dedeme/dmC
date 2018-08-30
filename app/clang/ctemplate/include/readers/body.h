// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Strucuture body reader.<br>
/// Definition:
///   PARAM+
///   -
///   VAR+
///   -
///   FUNC+

#ifndef READERS_BODY_H
  #define READERS_BODY_H

#include "data.h"
#include "RW.h"

/// Reads a structure body;
Body *body_read(RW *rw, Head *head);

#endif

