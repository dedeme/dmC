// Copyright 24-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef PROCESS_H
  #define PROCESS_H

#include "dmc/std.h"
#include "Model.h"

/// Processes a flea model in a group and return 1 if more than one flea
/// was selected.
int process_run(Model *fmodel);

#endif
