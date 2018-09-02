// Copyright 31-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef CLEAN_H
  #define CLEAN_H

#include "dmc/std.h"
#include "tp/P.h"

/// Cleans 'file' and send the result to target.<p>
/// 'file' is a relative path.
void clean_run(P *file, char *target);

#endif
