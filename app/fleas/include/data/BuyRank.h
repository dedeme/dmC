// Copyright 28-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Sorter of buy orders

#ifndef DATA_BUYRANK_H
  #define DATA_BUYRANK_H

#include "dmc/std.h"
#include "Abuy.h"

///
typedef struct buyRank_BuyRank BuyRank;

///
BuyRank *buyRank_new();

/// Adds a new order
void buyRank_add(BuyRank *this, Buy *buy, double mark);

/// Returns the added orders sorted
Abuy *buyRank_list(BuyRank *this);

#endif
