// Copyright 17-Feb-2018 ºDeme
// GNU Buyeral Public License - V3 <http://www.gnu.org/licenses/>

#include "families/UdCalc.h"
#include <dm.h>

struct udcalc_UdCalc {
  size_t ud_len;
  double mbuy;
  double msell;
  double *qs; // data
  double *start;
  double *end;
  double *ud_end;
  double *data_end;
};

UdCalc *udcalc_new(
  size_t ud_len,
  double mbuy,
  double msell,
  size_t quotes_len
) {
  UdCalc *this = MALLOC(UdCalc);
  double *qs = GC_MALLOC(sizeof(double) * quotes_len);
  this->ud_len = ud_len;
  this->mbuy = mbuy;
  this->msell = msell;
  this->qs = qs;
  this->start = qs;
  this->end = qs;
  this->ud_end = qs + ud_len;
  this->data_end = qs + quotes_len;

  return this;
}

inline
double *udcalc_values(UdCalc *this) {
  return this->start;
}

enum udcalc_Result  udcalc_add(UdCalc *this, double quote) {
  if (this->end < this->ud_end - 1) {
    if (quote <= 0) {
      --this->data_end;
    } else {
      *this->end++ = quote;
    }
    return UDCALC_NOT_VALID;
  }
  if (quote <= 0) {
    --this->data_end;
    return UDCALC_NOT_VALID;
  }
  if (this->end >= this->data_end) {
    THROW "this->end >= this->data_end" _THROW
  }
  double old_quote = *this->start++;
  *this->end++ = quote;
  return quote > old_quote * this->mbuy ? UDCALC_BUY
    : quote < old_quote * this->msell ? UDCALC_SELL
    : UDCALC_NOP
  ;
}
