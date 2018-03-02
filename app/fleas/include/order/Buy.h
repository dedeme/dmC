// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Buy order

#ifndef BUY_H
  # define BUY_H

#include <dm/Json.h>
#include <stdlib.h>
#include "Nick.h"
#include "market/Quote.h"

///
typedef struct buy_Buy Buy;

///
size_t buy_nick(Buy *this);

///
double buy_money(Buy *this);

/// buy_do executes a Buy. 'qs' are the quotes of day.<br>
/// It returns stocks_r and costs_r.
void buy_do(
  size_t *stocks_r,
  double *cost_r,
  Buy *this,
  Quote **qs
);

///
Json *buy_serialize(Buy *this, Nicks *nicks);

// ---------------------------------------------------------

///
typedef Arr Buys;

///
Buys *buys_new(void);

///
void buys_add(Buys *this, size_t nick, double money);

///
Buy *buys_get(Buys *this, size_t ix);

#endif


