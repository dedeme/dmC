// Copyright 28-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Structure accessory to BuyRank

#ifndef DATA_BUYMARKED_H
  #define DATA_BUYMARKED_H

#include "dmc/std.h"
#include "market/Buy.h"

/*.-.*/

///
typedef struct buyMarked_BuyMarked BuyMarked;

///
BuyMarked *buyMarked_new(Buy *order, double ponderation);

///
Buy *buyMarked_order(BuyMarked *this);

///
double buyMarked_ponderation(BuyMarked *this);

/*.-.*/

#endif
