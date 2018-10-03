// Copyright 25-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Cut values calculator.

#ifndef UTILS_CUTCALCULATOR_H
  #define UTILS_CUTCALCULATOR_H

#include "dmc/std.h"
#include "Flea.h"

typedef struct _cutCalculator_CutCalculator CutCalculator;

/// CutCalcuator calculates a value over which remains 'cut_number' values.
///   cut_number: Number of fleas to save.
void cutCalculator_init(int cut_number);

/// Adds a new value to calculate.
void cutCalculator_add(double value);

/// Returns cut value
double cutCalculator_cut_value(void);

#endif
