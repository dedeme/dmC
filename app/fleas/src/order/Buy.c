// Copyright 17-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "order/Buy.h"
#include <dm/dm.h>
#include "Quote.h"
#include "order/fees.h"

struct buy_Buy {
  size_t nick;
  double money;
};

static Buy *new(size_t nick, double money) {
  Buy *this = MALLOC(Buy);
  this->nick = nick;
  this->money = money;
  return this;
}

inline
size_t buy_nick(Buy *this) {
  return this->nick;
}

inline
double buy_money(Buy *this){
  return this->money;
}

void buy_do(
  size_t *stocks_r,
  double *cost_r,
  Buy *this,
  Quote **qs
) {
	Quote *q = qs[this->nick];
	double price = quote_open(q);
	if (price > 0) {
    double order_money = this->money;
		double money =  order_money - fees_app(order_money);
		size_t stocks = (size_t)(money / price);

		for (;;) {
			double cost = stocks * price;
      cost += fees_app(cost);
			if (cost <= order_money) {
        *stocks_r = stocks;
        *cost_r = cost;
        return;
			}
			--stocks;
		}

	}
	*stocks_r = 0;
  *cost_r = 0.0;
}

Json *buy_serialize(Buy *this, Nicks *nicks) {
  Arr/*Json*/ *serial = arr_new();

  jarr_astring(serial, nick_id(nicks_get(nicks,this->nick)));
  jarr_adouble(serial, this->money, 2);

  return json_warray(serial);
}

// ---------------------------------------------------------

inline
Buys *buys_new(void) {
  return arr_new();
}


inline
void buys_add(Buys *this, size_t nick, double money) {
  arr_add(this, new(nick, money));
}

inline
Buy *buys_get(Buys *this, size_t ix) {
  return arr_get(this, ix);
}

