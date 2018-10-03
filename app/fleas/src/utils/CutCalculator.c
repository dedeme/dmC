// Copyright 25-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "utils/CutCalculator.h"
#include "dmc/std.h"
#include "data/HistoricEntry.h"

// Ordered from less to greater
static double *values = NULL;
static int size = 0;

void cutCalculator_init(int cut_number) {
  size = cut_number;
  values = ATOMIC(size * sizeof(double));
  RANGE0(i, size) {
    values[i] = 0;
  }_RANGE
}

void cutCalculator_add(double value) {
  int ix = size - 1;
  if (values[ix] >= value) {
    return;
  }
  --ix;
  while (ix >= 0) {
    if (values[ix] >= value) {
      values[ix + 1] = value;
      return;
    }
    values[ix + 1] = values[ix];
    --ix;
  }
  values[0] = value;
}

double cutCalculator_cut_value(void) {
  return values[size - 1];
}
