// Copyright 30-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "families/UpDownH.h"
#include "families/UpDown.h"

Gen *upDownH_prototype(void) {
  double gens[4] = {1000, 40, 0.005, 0.005};
  return gen_new(4, gens);
}

///
void upDownH_init(Gen *gens, int companies_number) {
  upDown_init(gens, companies_number);
}

/// Returns 'buys' and nicks to sell list from 'quotes'
///   buys: Sorted array with buy orders to execute. [Return]
///   nicks_to_sell: array with company nicks to sell. [Return]
///   nicks_to_sell_len: Length of 'nicks_to_sell' [Return]
///   quotes: Day quotes.
void upDownH_operate(
  Abuy **buys,
  int **nicks_to_sell,
  int *nicks_to_sell_len,
  Quote **quotes
) {
  upDown_operate(buys, nicks_to_sell, nicks_to_sell_len, quotes);
}
