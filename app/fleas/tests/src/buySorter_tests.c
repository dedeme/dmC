// Copyright 28-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "buySorter_tests.h"
#include "BuySorter.h"
#include <assert.h>

void buySorter_tests() {
  puts("BuySorter tests");

  BuySorter *bs = buySorter_new(3);
  Buy *b1 = buy_new(1, 10, 2.5);
  Buy *b2 = buy_new(2, 10, 2.5);
  Buy *b3 = buy_new(3, 10, 2.5);

  buySorter_add(bs, b1, 2);
  buySorter_add(bs, b2, 1);
  buySorter_add(bs, b3, 3);

  // Arr[Buy]
  Arr *a_bs = buySorter_free_new(bs);
  assert(buy_nick(arr_get(a_bs, 0)) == 3);
  assert(buy_nick(arr_get(a_bs, 1)) == 1);
  assert(buy_nick(arr_get(a_bs, 2)) == 2);

  arr_free(a_bs);

  puts("    Finished");
}

