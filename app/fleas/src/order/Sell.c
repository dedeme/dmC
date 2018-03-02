// Copyright 17-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/*.+.
@struct: -Sell
  nick: size_t
  stocks: size_t
*/

#include "order/Sell.h"
#include <dm/dm.h>
#include "market/Quote.h"
#include "order/fees.h"

/*.-.*/
struct sell_Sell {
  size_t nick;
  size_t stocks;
};

static Sell *new(size_t nick, size_t stocks) {
  Sell *this = MALLOC(Sell);
  this->nick = nick;
  this->stocks = stocks;
  return this;
}

inline
size_t sell_nick(Sell *this) {
  return this->nick;
}

inline
size_t sell_stocks(Sell *this){
  return this->stocks;
}
/*.-.*/

double sell_do(Sell *this, Quote **qs) {
	Quote *q = qs[this->nick];
	double price = quote_open(q);
	if (price > 0) {
    return sell_income(this->stocks, price);
  }
  return -1.0;
}

Json *sell_serialize(Sell *this, Nicks *nicks) {
  Arr/*Json*/ *serial = arr_new();

  jarr_astring(serial, nick_id(nicks_get(nicks,this->nick)));
  jarr_auint(serial, this->stocks);

  return json_warray(serial);
}

double sell_income(size_t stocks, double price) {
  double inc = stocks * price;
  return inc - fees_app(inc);
}

// ---------------------------------------------------------

inline
Sells *sells_new(void) {
  return arr_new();
}

inline
void sells_add(Sells *this, size_t nick, size_t stocks) {
  arr_add(this, new(nick, stocks));
}

inline
Sell *sells_get(Sells *this, size_t ix) {
  return arr_get(this, ix);
}
