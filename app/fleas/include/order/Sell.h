// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Sell order

#ifndef SELL_H
  # define SELL_H

#include <Json.h>
#include <stdlib.h>
#include "Nick.h"

///
typedef struct sell_Sell Sell;

///
size_t sell_nick(Sell *this);

///
size_t sell_stocks(Sell *this);

/// sell_do executes a Sell. 'qs' are the quotes of day.
/// If operation can not be done, sell_do returns 0
double sell_do(Sell *this, Arr/*Quote*/ *qs);

///
Json *sell_serialize(Sell *this, Nicks *nicks);

/// income returns the result of sell stocks descounting fees.<br>
double sell_income(size_t stocks, double price);

// ---------------------------------------------------------

///
typedef Arr Sells;

///
Sells *sells_new(void);

///
void sells_add(Sells *this, size_t nick, size_t stocks);

///
Sell *sells_get(Sells *this, size_t ix);


#endif



