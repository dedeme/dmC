// Copyright 22-Jan-2018 ºDeme
// GNU Buyeral Public License - V3 <http://www.gnu.org/licenses/>

#include "families/WmaCalc.h"
#include <dmc/all.h>

struct wmacalc_WmaCalc {
  size_t avg_len;
  double mbuy;
  double msell;
  double n;
  double reason;
  double sum;
};

WmaCalc *wmacalc_new(
  size_t avg_len,
  double mbuy,
  double msell
) {
  WmaCalc *this = MALLOC(WmaCalc);
  this->avg_len = avg_len;
  this->mbuy = mbuy;
  this->msell = msell;
  this->n = 0;
  this->reason = ((double)avg_len - 1.0) / (double)avg_len;
  this->sum = 0;

  return this;
}

inline
double wmacalc_sum(WmaCalc *this) {
  return this->sum;
}

enum wmacalc_Result  wmacalc_add(WmaCalc *this, double quote) {
  if (this->n < this->avg_len - 1) {
    if (quote > 0) {
      this->sum += quote;
      ++this->n;
    }
    return WMACALC_NOT_VALID;
  }
  if (quote <= 0) {
    return WMACALC_NOT_VALID;
  }
  if (this->n < this->avg_len) {
    this->sum += quote;
    ++this->n;
  } else {
    this->sum = this->sum * this->reason + quote;
  }

  double sum = this->sum;

  double mulquote = quote * (double)this->avg_len;
  return mulquote > sum * this->mbuy ? WMACALC_BUY
    : mulquote < sum * this->msell ? WMACALC_SELL
    : WMACALC_NOP
  ;
}
