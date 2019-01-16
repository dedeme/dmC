// Copyright 14-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Nick.h"
#include "dmc/date.h"
#include "io.h"

/* .
-Nick: serial
  id: char *
  # Hours/Quotes
  hqs: Arr *:: hourQuote
  #Stocks number if nick is bought or 0 otherwise.
  stocks: int
  #Price of bought stocks.
  price: double::_d4
  # (> 0) -> Suport (buying). (< 0) -> Resitence (selling). (=0) -> Not operation.
  signal: double:: _d4
===
-HourQuote: serial
  h: char *
  q: double::_d4
*/

/*--*/

struct nick_Nick {
  char *id;
  Arr *hqs;
  int stocks;
  double price;
  double signal;
};

Nick *_nick_new(
  char *id,
  Arr *hqs,
  int stocks,
  double price,
  double signal
) {
  Nick *this = malloc(sizeof(Nick));
  this->id = id;
  this->hqs = hqs;
  this->stocks = stocks;
  this->price = price;
  this->signal = signal;
  return this;
}

void nick_free(Nick *this) {
  free(this->id);
  arr_free(this->hqs);
  free(this);
};

char *nick_id(Nick *this) {
  return this->id;
}

Arr *nick_hqs(Nick *this) {
  return this->hqs;
}

int nick_stocks(Nick *this) {
  return this->stocks;
}

double nick_price(Nick *this) {
  return this->price;
}

double nick_signal(Nick *this) {
  return this->signal;
}

Js *nick_to_js_new(Nick *this) {
  // Arr[Js]
  Arr *a = arr_new(free);
  arr_push(a, js_ws_new(this->id));
  arr_push(a, arr_to_js_new(this->hqs, (FTO)hourQuote_to_js_new));
  arr_push(a, js_wi_new(this->stocks));
  arr_push(a, js_wd_new(this->price, 4));
  arr_push(a, js_wd_new(this->signal, 4));
  Js *r = js_wa_new(a);
  arr_free(a);
  return r;
}

Nick *nick_from_js_new(Js *js) {
  // Arr[Js]
  Arr *a = js_ra_new(js);
  int i = 0;
  char *id = js_rs_new(arr_get(a, i++));
  Arr *hqs = arr_from_js_new(arr_get(a, i++), (FFROM)hourQuote_from_js_new, (FPROC)hourQuote_free);
  int stocks = js_ri(arr_get(a, i++));
  double price = js_rd(arr_get(a, i++));
  double signal = js_rd(arr_get(a, i++));
  arr_free(a);
  return _nick_new(id, hqs, stocks, price, signal);
}

struct hourQuote_HourQuote {
  char *h;
  double q;
};

HourQuote *_hourQuote_new(char *h, double q) {
  HourQuote *this = malloc(sizeof(HourQuote));
  this->h = h;
  this->q = q;
  return this;
}

void hourQuote_free(HourQuote *this) {
  free(this->h);
  free(this);
};

char *hourQuote_h(HourQuote *this) {
  return this->h;
}

double hourQuote_q(HourQuote *this) {
  return this->q;
}

Js *hourQuote_to_js_new(HourQuote *this) {
  // Arr[Js]
  Arr *a = arr_new(free);
  arr_push(a, js_ws_new(this->h));
  arr_push(a, js_wd_new(this->q, 4));
  Js *r = js_wa_new(a);
  arr_free(a);
  return r;
}

HourQuote *hourQuote_from_js_new(Js *js) {
  // Arr[Js]
  Arr *a = js_ra_new(js);
  int i = 0;
  char *h = js_rs_new(arr_get(a, i++));
  double q = js_rd(arr_get(a, i++));
  arr_free(a);
  return _hourQuote_new(h, q);
}

/*--*/

void nick_new(char *nick, PfValue *pfv, double signal) {
  Nick *nk = _nick_new(
    str_new(nick),
    arr_new((FPROC)hourQuote_free),
    pfv ? pfValue_stocks(pfv) : 0,
    pfv ? pfValue_price(pfv) : 0.0,
    signal
  );
  char *js = (char *)nick_to_js_new(nk);
  io_nick_write(nick, js);
  free(js);
  nick_free(nk);
}

void nick_update(char *nick, double quote) {
  Js *js = (Js *)io_nick_read_new(nick);
  Nick *nk = nick_from_js_new(js);
  arr_push(nk->hqs, _hourQuote_new(date_f_new(date_now(), "%H"), quote));
  free(js);
  js = nick_to_js_new(nk);
  io_nick_write(nick, (char *)js);
  free(js);
  nick_free(nk);
}
