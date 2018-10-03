// Copyright 25-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "utils/CutFleaCalculator.h"
#include "dmc/std.h"
#include "data/HistoricEntry.h"

// Ordered from less to greater
static double *ponds = NULL;
static HistoricEntry **entries = NULL;
static int size = 0;

void cutFleaCalculator_init(int cut_number) {
  size = cut_number;
  ponds = ATOMIC(size * sizeof(double));
  entries = GC_MALLOC(size * sizeof(HistoricEntry *));
  RANGE0(i, size) {
    ponds[i] = 0;
    entries[i] = NULL;
  }_RANGE
}

void cutFleaCalculator_add(Flea *f) {
  double p = flea_ponderation(f);
  HistoricEntry *h = historicEntry_new(
    f, p, flea_assets(f), flea_buys(f), flea_sells(f)
  );
  int ix = size - 1;
  if (ponds[ix] >= p) {
    return;
  }
  --ix;
  while (ix >= 0) {
    if (ponds[ix] >= p) {
      ponds[ix + 1] = p;
      entries[ix + 1] = h;
      return;
    }
    ponds[ix + 1] = ponds[ix];
    entries[ix + 1] = entries[ix];
    --ix;
  }
  ponds[0] = p;
  entries[0] = h;
}

Arr/*HistoricEntry*/ *cutFleaCalculator_fleas(void) {
  Arr *r = arr_new_buf(size + 1);
  RANGE0(i, size) {
    arr_add(r, entries[i]);
  }_RANGE
  return r;
}
