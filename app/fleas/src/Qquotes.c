// Copyright 26-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Qquotes.h"

struct qquotes_Qquotes {
  double *quotes;
  int days;
};

Qquotes *qquotes_new (int days) {
  Qquotes *this = malloc(sizeof(Qquotes));
  this->quotes = malloc(sizeof(double) * days);
  this->days = days;
  double *p = this->quotes;
  REPEAT(days)
    *p++ = -3;
  _REPEAT
  return this;
}

void qquotes_free (Qquotes *this) {
  if (this) {
    free(this->quotes);
    free(this);
  }
}

int qquotes_waiting (Qquotes *this) {
  return *this->quotes < 0;
}

void qquotes_add (Qquotes *this, double quote) {
  double *p = this->quotes;
  double *p1 = p + 1;
  REPEAT(this->days - 1)
    *p++ = *p1++;
  _REPEAT
  *p = quote;
}

double qquotes_max (Qquotes *this) {
  double *p = this->quotes;
  double r = *p++;
  if (r < -2) return -1;
  double v;
  REPEAT(this->days - 1)
    v = *p++;
    if (v > r) r = v;
  _REPEAT
  return r;
}

double qquotes_min (Qquotes *this) {
  double *p = this->quotes;
  double r = *p++;
  if (r < -2) return -1;
  double v;
  REPEAT(this->days - 1)
    v = *p++;
    if (r >= 0) {
      if (v >= 0 && v < r) r = v;
    } else {
      if (v >= 0) r = v;
    }
  _REPEAT
  return r;
}

double qquotes_avg (Qquotes *this) {
  double *p = this->quotes;
  double sum = *p++;
  int n = 1;
  if (sum < -2) return -1;
  if (sum < 0) {
    n = 0;
    sum = 0;
  }
  double v;
  REPEAT(this->days - 1)
    v = *p++;
    if (v >= 0) {
      ++n;
      sum += v;
    }
  _REPEAT
  return n > 0 ? sum / n : -1;
}

char *qquotes_to_str_new(Qquotes *this) {
  double *p = this->quotes;
  int days = this->days;

  Buf *bf = buf_new();
  buf_cadd(bf, '[');
  if (days) {
    char *s = str_f_new("%.2f", *p++);
    buf_add(bf, s);
    free(s);
    REPEAT(days - 1)
      buf_add(bf, ", ");
      s = str_f_new("%.2f", *p++);
      buf_add(bf, s);
      free(s);
    _REPEAT
  }
  buf_cadd(bf, ']');
  char *r = buf_to_str_new(bf);
  buf_free(bf);
  return r;
}
