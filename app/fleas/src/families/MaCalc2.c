// Copyright 17-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <dmc/all.h>
#include "families/MaCalc2.h"

struct macalc2_MaCalc2 {
  double *qs; // data
  double *next;
  double *data_end;

  size_t slen;
  double *sstart;
  double *send; // sstart + slen
  double ssum;

  size_t llen;
  double *lstart;
  double *lend; // lstart + llen
  double lsum;
};

MaCalc2 *macalc2_new(
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

  MaCalc2 *this = MALLOC(MaCalc2);
  double *qs = GC_MALLOC(sizeof(double) * quotes_len);

  this->qs = qs;
  this->next = qs;
  this->data_end = qs + quotes_len;

  this->slen = short_avg_len;
  this->sstart = qs;
  this->send = qs + short_avg_len;
  this->ssum = 0.0;

  this->llen = long_avg_len;
  this->lstart = qs;
  this->lend = qs + long_avg_len;
  this->lsum = 0.0;

  return this;
}

inline
double macalc2_short_sum(MaCalc2 *this) {
  return this->ssum;
}

inline
double macalc2_long_sum(MaCalc2 *this) {
  return this->lsum;
}

inline
double *macalc2_short_values(MaCalc2 *this) {
  return this->sstart;
}

inline
double *macalc2_long_values(MaCalc2 *this) {
  return this->lstart;
}

bool macalc2_add(double *inc, MaCalc2 *this, double quote) {
  if (this->next < this->send - 1) {
    if (quote > 0) {
      this->ssum += quote;
    }
  } else {
    if (quote > 0) {
      if (this->next < this->send) {
        this->ssum += quote;
      } else {
        this->ssum += quote - *this->sstart++;
      }
    }
  }

  if (this->next < this->lend - 1) {
    if (quote <= 0) {
      --this->data_end;
    } else {
      *this->next++ = quote;
      this->lsum += quote;
    }
    *inc = -10;
    return false;
  }

  if (quote <= 0) {
    --this->data_end;
    *inc = -10;
    return true;
  }
  if (this->next < this->lend) {
    this->lsum += quote;
  } else {
    this->lsum += quote - *this->lstart++;
  }

  if (this->next >= this->data_end) {
    THROW "this->next >= this->data_end" _THROW
  }
  *this->next++ = quote;

  double vlong = this->lsum * this->slen;
  *inc = (this->ssum * this->llen - vlong) / vlong;
  return true;
}
