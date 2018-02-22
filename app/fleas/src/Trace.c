// Copyright 17-Dec-2017 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Trace.h"
#include <dm.h>
#include "Portfolio.h"

struct trace_Trace {
  char *date;
	Quote *quote;
	double beforeCash;
	Portfolio *beforePortfolio;
	char *nick;
	double cash;
	uint stocks;
	double afterCash;
	Portfolio *afterPortfolio;
	Json *family_data;
};

Trace *trace_new(
  char *date,
	Quote *quote,
	double beforeCash,
	Portfolio *beforePortfolio,
	char *nick,
	double cash, // if order is sell its value is 0
	uint stocks, // if order is buy its value is 0
	double afterCash,
	Portfolio *afterPortfolio,
	Json *family_data
) {
  Trace *this = MALLOC(Trace);
  this->date = date;
  this->quote = quote;
  this->beforeCash = beforeCash;
  this->beforePortfolio = beforePortfolio;
  this->nick = nick;
  this->cash = cash;
  this->stocks = stocks;
  this->afterCash = afterCash;
  this->afterPortfolio = afterPortfolio;
  this->family_data = family_data;
  return this;
}

inline
Trace *trace_copy(Trace *this) {
  return trace_new(
    this->date,
    this->quote,
    this->beforeCash,
    this->beforePortfolio,
    this->nick,
    this->cash,
    this->stocks,
    this->afterCash,
    this->afterPortfolio,
    this->family_data
  );
}

Json *trace_serialize(Trace *this, Nicks *nicks) {
  Arr/*Json*/ *serial = arr_new();

  jarr_astring(serial, this->date);
  arr_add(serial, quote_serialize(this->quote));
  jarr_adouble(serial, this->beforeCash, 2);
  arr_add(serial, portfolio_serialize(this->beforePortfolio, nicks));
  jarr_astring(serial, this->nick);
  jarr_adouble(serial, this->cash, 2);
  jarr_auint(serial, this->stocks);
  jarr_adouble(serial, this->afterCash, 2);
  arr_add(serial, portfolio_serialize(this->afterPortfolio, nicks));
  arr_add(serial, this->family_data);

  return json_warray(serial);
}

