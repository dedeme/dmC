// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Stat.h"
#include <dm.h>
#include <DEFS.h>

struct gen_Stat {
  double cash;
  double buys;
  double sells;
};

Stat *stat_new(void) {
  Stat *this = MALLOC(Stat);
  this->cash = 0.0;
  this->buys = 0.0;
  this->sells = 0.0;
  return this;
}

Stat *stat_copy(Stat *this) {
  Stat *other = MALLOC(Stat);
  other->cash = this->cash;
  other->buys = this->buys;
  other->sells = this->sells;
  return other;
}

inline
double stat_cash(Stat *this) {
  return this->cash;
}

inline
double stat_buys(Stat *this) {
  return this->buys;
}

inline
double stat_sells(Stat *this) {
  return this->sells;
}

void stat_setup(Stat *this, uint n, double cash, double nbuys, double nsells) {
  if (n > STAT_INTERVAL) {
    n = STAT_INTERVAL;
  }
  double n0 = (double)n;
  double n1 = n0 + 1.0;
  this->cash = (this->cash * n0 + cash) / n1;
  this->buys = (this->buys * n0 + nbuys) / n1;
  this->sells = (this->sells * n0 + nsells) / n1;
}

Json *stat_serialize(Stat *this) {
  Arr/*Json*/ *serial = arr_new();

  jarr_adouble(serial, this->cash, 2);
  jarr_adouble(serial, this->buys, 2);
  jarr_adouble(serial, this->sells, 2);

  return json_warray(serial);
}

Stat *stat_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  Stat *this = MALLOC(Stat);

  size_t i = 0;
  this->cash = jarr_gdouble(serial, i++);
  this->buys = jarr_gdouble(serial, i++);
  this->sells = jarr_gdouble(serial, i++);

  return this;
}

