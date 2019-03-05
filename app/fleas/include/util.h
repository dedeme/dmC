// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef UTIL_H
  #define UTIL_H

#include "dmc/std.h"
#include "Model.h"

/// Calculates the actual value of a gen parameter.
///   mx: Maximun value of actual parameter
///   mn: Minimum value of actual parameter
///   value: Gen parameter (between (0 and 1]
double util_param(double mx, double mn, double value);

///
void util_print_model_name(Model *model);

#endif
