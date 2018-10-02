// Copyright 28-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef FAMILIES_UPDOWN_H
  #define FAMILIES_UPDOWN_H

#include "dmc/std.h"
#include "Gen.h"
#include "market/Quote.h"
#include "data/Abuy.h"

/*.-.*/


/*.-.*/

///
Gen *upDown_prototype(void);

///
void upDown_init(Gen *gens, int companies_number);

/// Returns 'buys' and nicks to sell list from 'quotes'
///   buys: Sorted array with buy orders to execute. [Return]
///   nicks_to_sell: array with company nicks to sell. [Return]
///   nicks_to_sell_len: Length of 'nicks_to_sell' [Return]
///   quotes: Day quotes.
void upDown_operate(
  Abuy **buys,
  int **nicks_to_sell,
  int *nicks_to_sel_len,
  Quote **quotes
);

#endif
