// Copyright 17-Feb-2018 ºDeme
// GNU Buyeral Public License - V3 <http://www.gnu.org/licenses/>

#include "families/MaCalc.h"
#include <dmc/all.h>

struct macalc_MaCalc {
  size_t avg_len;
  double mbuy;
  double msell;
  double *qs; // data
  double *start;
  double *end;
  double *avg_end;
  double *data_end;
  double sum;
};

MaCalc *macalc_new(
  size_t avg_len,
  double mbuy,
  double msell,
  size_t quotes_len
) {
  MaCalc *this = MALLOC(MaCalc);
  double *qs = GC_MALLOC(sizeof(double) * quotes_len);
  this->avg_len = avg_len;
  this->mbuy = mbuy;
  this->msell = msell;
  this->qs = qs;
  this->start = qs;
  this->end = qs;
  this->avg_end = qs + avg_len;
  this->data_end = qs + quotes_len;
  this->sum = 0;

  return this;
}

inline
double macalc_sum(MaCalc *this) {
  return this->sum;
}

inline
double *macalc_values(MaCalc *this) {
  return this->start;
}

enum macalc_Result  macalc_add(MaCalc *this, double quote) {
  if (this->end < this->avg_end - 1) {
    if (quote <= 0) {
      --this->data_end;
    } else {
      *this->end++ = quote;
      this->sum += quote;
    }
    return MACALC_NOT_VALID;
  }
  if (quote <= 0) {
    --this->data_end;
    return MACALC_NOT_VALID;
  }
  if (this->end >= this->data_end) {
    THROW "this->end >= this->data_end" _THROW
  }
  if (this->end < this->avg_end) {
    this->sum += quote;
  } else {
    this->sum += quote - *this->start++;
  }
  *this->end++ = quote;
  double sum = this->sum;
  double mulquote = quote * (double)this->avg_len;
  return mulquote > sum * this->mbuy ? MACALC_BUY
    : mulquote < sum * this->msell ? MACALC_SELL
    : MACALC_NOP
  ;
}
