// Copyright 25-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Cut fleas calculator.

#ifndef UTILS_CUT_FLEA_CALCULATOR_H
  #define UTILS_CUT_FLEA_CALCULATOR_H

#include "dmc/std.h"
#include "Flea.h"

typedef struct _cutFleaCalculator_CutFleaCalculator CutFleaCalculator;

/// CutCalcuator save a 'cut_number' of the best fleas.
///   cut_number: Number of fleas to save.
void cutFleaCalculator_init(int cut_number);

/// Adds a new flea value to calculate.
void cutFleaCalculator_add(Flea *f);

/// Returns the fleas saved
Arr/*HistoricEntry*/ *cutFleaCalculator_fleas(void);

#endif
