// Copyright 16-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Quote.h"
#include "DEFS.h"
#include <dm/dm.h>

struct quote_Quote {
  double open;
  double close;
  double max;
  double min;
  size_t vol;
};

Quote *quote_from_str(char *quote) {
  Arr/*char*/ *qparts = str_csplit(quote, ':');
  Quote *this = malloc(sizeof(Quote));
  this->open = atof(arr_get(qparts, 1));
  this->close = atof(arr_get(qparts, 2));
  this->max = atof(arr_get(qparts, 3));
  this->min = atof(arr_get(qparts, 4));
  this->vol = atoi(arr_get(qparts, 5));
  return this;
}

inline
double quote_open(Quote *q) {
  return q->open;
}

inline
double quote_close(Quote *q) {
  return q->close;
}

inline
double quote_max(Quote *q) {
  return q->max;
}

inline
double quote_min(Quote *q) {
  return q->min;
}

inline
size_t quote_vol(Quote *q) {
  return q->vol;
}

Json *quote_serialize(Quote *this) {
  Arr/*Json*/ *serial = arr_new();

  jarr_adouble(serial, this->open, 4);
  jarr_adouble(serial, this->close, 4);
  jarr_adouble(serial, this->max, 4);
  jarr_adouble(serial, this->min, 4);
  jarr_auint(serial, this->vol);

  return json_warray(serial);
}

Quote *quote_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  size_t i = 0;
  Quote *this = MALLOC(Quote);
  this->open = jarr_gdouble(serial, i++);
  this->close = jarr_gdouble(serial, i++);
  this->max = jarr_gdouble(serial, i++);
  this->min = jarr_gdouble(serial, i++);
  this->vol = jarr_guint(serial, i++);
  return this;
}

// -------------------------------------------------------------------

static Quote **quotes = NULL;

Quote **quotes_get() {
  if (quotes) {
    return quotes;
  }
  quotes = malloc(sizeof(Quote *) * QUOTES_NUMBER * NICKS_NUMBER);
  return quotes;
}

///
void quotes_free() {
  Quote **p = quotes;
  REPEAT(QUOTES_NUMBER * NICKS_NUMBER) {
    free(*p++);
  }_REPEAT
  free(quotes);
  quotes = NULL;
}
