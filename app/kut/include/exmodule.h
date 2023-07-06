// Copyright 02-Jul-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Adds-on modules management.

#ifndef EXMODULE_H
  #define EXMODULE_H

#include "exp.h"

/// Execute adds-on 'c' module.
/// Throws EXC_KUT if 'fn' does not exist.
Exp *exmodule_run (char *fn, Exp *data);

#endif
