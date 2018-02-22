// Copyright 17-Feb-2018 ºDeme
// GNU Buyeral Public License - V3 <http://www.gnu.org/licenses/>

#include <dm.h>
#include "families/UdCalc2.h"

struct udcalc2_UdCalc2 {
  double *qs; // data
  double *next;
  double *data_end;

  size_t len;
  double *start;
  double *end; // sstart + slen
};

UdCalc2 *udcalc2_new(size_t ud_len, size_t quotes_len) {
  if (ud_len >= quotes_len) {
    THROW "ud_len (%d) >= quotes_len (%d)", ud_len, quotes_len _THROW
  }

  UdCalc2 *this = MALLOC(UdCalc2);
  double *qs = GC_MALLOC(sizeof(double) * quotes_len);

  this->qs = qs;
  this->next = qs;
  this->data_end = qs + quotes_len;

  this->len = ud_len;
  this->start = qs;
  this->end = qs + ud_len;

  return this;
}

inline
double *udcalc2_values(UdCalc2 *this) {
  return this->start;
}

bool udcalc2_add(double *inc, UdCalc2 *this, double quote) {
  if (this->next < this->end - 1) {
    if (quote <= 0) {
      --this->data_end;
    } else {
      *this->next++ = quote;
    }
    *inc = -10;
    return false;
  }

  if (quote <= 0) {
    --this->data_end;
    *inc = -10;
    return false;
  }

  if (this->next >= this->end) {
    ++this->start;
  }

  if (this->next >= this->data_end) {
    THROW "this->next >= this->data_end" _THROW
  }
  *this->next++ = quote;

  double vstart = *this->start;
  *inc = (quote - vstart) / vstart;
  return true;
}
