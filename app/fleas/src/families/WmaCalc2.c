// Copyright 27-Jan-2018 ºDeme
// GNU Buyeral Public License - V3 <http://www.gnu.org/licenses/>

#include <dm.h>
#include "families/WmaCalc2.h"

struct wmacalc2_WmaCalc2 {
  size_t slen;
  size_t sn;
  double sreason;
  double ssum;

  size_t llen;
  size_t ln;
  double lreason;
  double lsum;
};

WmaCalc2 *wmacalc2_new(
  size_t short_avg_len,
  size_t long_avg_len,
  size_t quotes_len
) {
  if (short_avg_len >= long_avg_len) {
    THROW
      "short_avg_len (%d) >= long_avg_len (%d)",
      short_avg_len, long_avg_len
    _THROW
  }

  if (long_avg_len >= quotes_len) {
    THROW
      "long_avg_len (%d) >= quotes_len (%d)",
      long_avg_len, quotes_len
    _THROW
  }

  WmaCalc2 *this = MALLOC(WmaCalc2);

  this->slen = short_avg_len;
  this->sn = 0;
  this->sreason = ((double)short_avg_len - 1.0) / (double)short_avg_len;
  this->ssum = 0.0;

  this->llen = long_avg_len;
  this->ln = 0;
  this->lreason = ((double)long_avg_len - 1.0) / (double)long_avg_len;
  this->lsum = 0.0;

  return this;
}

inline
double wmacalc2_short_sum(WmaCalc2 *this) {
  return this->ssum;
}

inline
double wmacalc2_long_sum(WmaCalc2 *this) {
  return this->lsum;
}

bool wmacalc2_add(double *inc, WmaCalc2 *this, double quote) {
  if (this->sn < this->slen - 1) {
    if (quote > 0) {
      this->ssum += quote;
      ++this->sn;
    }
  } else {
    if (quote > 0) {
      if (this->sn < this->slen) {
        this->ssum += quote;
        ++this->sn;
      } else {
        this->ssum = this->ssum * this->sreason + quote;
      }
    }
  }

  if (this->ln < this->llen - 1) {
    if (quote > 0) {
      this->lsum += quote;
      ++this->ln;
    }
    *inc = -10;
    return false;
  }

  if (quote > 0) {
    if (this->ln < this->llen) {
      this->lsum += quote;
      ++this->ln;
    } else {
      this->lsum = this->lsum * this->lreason + quote;
    }
  } else {
    *inc = -10;
    return true;
  }

  double vlong = this->lsum * this->slen;
  *inc = (this->ssum * this->llen - vlong) / vlong;
  return true;
}
