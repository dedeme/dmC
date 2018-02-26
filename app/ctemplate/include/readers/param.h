// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Parameter reader.<br>
/// Definition:
///   [+|-]id: type [: fserial]

#ifndef READERS_PARAM_H
  #define READERS_PARAM_H

#include "data.h"
#include "RW.h"

///
Param *param_read(RW *rw, Head *head);

#endif
