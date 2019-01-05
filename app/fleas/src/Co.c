// Copyright 27-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Co.h"

/* .
-Co
  closes: double *: !free
  # Last close
  last_close: double *: _i
  # Buying status
  @buying: int
  # Reference to calculate strips
  @ref: double
*/

/*--*/

struct co_Co {
  double *closes;
  double *last_close;
  int buying;
  double ref;
};

Co *_co_new(
  double *closes,
  double *last_close,
  int buying,
  double ref
) {
  Co *this = malloc(sizeof(Co));
  this->closes = closes;
  this->last_close = last_close;
  this->buying = buying;
  this->ref = ref;
  return this;
}

void co_free(Co *this) {
  free(this->closes);
  free(this);
};

double *co_closes(Co *this) {
  return this->closes;
}

double *co_last_close(Co *this) {
  return this->last_close;
}

int co_buying(Co *this) {
  return this->buying;
}

double co_ref(Co *this) {
  return this->ref;
}

void co_set_buying(Co *this, int v) {
  this->buying = v;
}

void co_set_ref(Co *this, double v) {
  this->ref = v;
}

/*--*/

Co *co_new(int ndays) {
  double *closes = malloc(sizeof(double) * ndays);
  double *bf_end = closes;
  REPEAT(ndays)
    *bf_end++ = -1;
  _REPEAT
  return _co_new(closes, bf_end - 1, 0, -1);
}

int co_add(Co *this, double close) {
  double *begin = this->closes;
  double *end = this->last_close;
  double *p = begin;
  while (p < end) {
    *p = *(p + 1);
    ++p;
  }
  *p = close;

  if (this->ref > 0) {
    if (*begin > 0) {
      if (this->buying) {
        if (*begin < this->ref) {
          this->ref = *begin;
        }
      } else {
        if (*begin > this->ref) {
          this->ref = *begin;
        }
      }
    }
    return close > 0;
  } else {
    if (*begin > 0) {
      this->ref = *begin;
      return close > 0;
    } else {
      return 0;
    }
  }
}

double co_up(Co *this, double strip) {
  double dif = (*this->last_close - this->ref) / this->ref;
  return dif > strip ? dif / strip : -1;
}

double co_down(Co *this, double strip) {
  double dif = (this->ref - *this->last_close) / this->ref;
  return dif > strip ? dif / strip : -1;
}

