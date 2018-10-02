// Copyright 25-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Cut fleas calculator.

#ifndef UTILS_CUTCALCULATOR_H
  #define UTILS_CUTCALCULATOR_H

#include "dmc/std.h"
#include "Flea.h"

typedef struct _cutCalculator_CutCalculator CutCalculator;

/// CutCalcuator save a 'cut_number' of the best fleas.
///   cut_number: Number of fleas to save.
void cutCalculator_init(int cut_number);

/// Adds a new flea value to calculate.
void cutCalculator_add(Flea *f);

/// Returns the fleas saved
Arr/*HistoricEntry*/ *cutCalculator_fleas(void);

#endif
