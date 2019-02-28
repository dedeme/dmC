// Copyright 26-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef CALCCHARTS_H
  #define CALCCHARTS_H

#include "dmc/std.h"
#include "Rs.h"
#include "Model.h"

/// Returns a json with the follow structure:
///   [
///     [
///       Nick: String,
///       profits: Number,
///       [
///         [
///           date: String,
///           close: Number,
///           ref: Number
///         ]
///       ]
///     ]
///   ]
char *calcCharts(Model *fmodel, Rs *rs);

#endif
