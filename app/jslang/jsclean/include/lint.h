// Copyright 07-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef LINT_H
  #define LINT_H

#include "dmc/std.h"

/// Returns lint errors or "". 'paths' is Arr[Path]
char *lint_run(Arr *paths);

#endif
