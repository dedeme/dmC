// Copyright 14-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Nick data

#ifndef NICK_H
  #define NICK_H

#include "dmc/std.h"
#include "dmc/Darr.h"
#include "dmc/Iarr.h"
#include "Server.h"
#include "pf.h"

/*--*/

///
typedef struct nick_Nick Nick;

///
void nick_free(Nick *this);

///
char *nick_id(Nick *this);

///  Hours/Quotes
Arr *nick_hqs(Nick *this);

/// Stocks number if nick is bought or 0 otherwise.
int nick_stocks(Nick *this);

/// Price of bought stocks.
double nick_price(Nick *this);

///  (> 0) -> Suport (buying). (< 0) -> Resitence (selling). (=0) -> Not operation.
double nick_signal(Nick *this);

///
Js *nick_to_js_new(Nick *this);

///
Nick *nick_from_js_new(Js *js);

///
typedef struct hourQuote_HourQuote HourQuote;

///
void hourQuote_free(HourQuote *this);

///
char *hourQuote_h(HourQuote *this);

///
double hourQuote_q(HourQuote *this);

///
Js *hourQuote_to_js_new(HourQuote *this);

///
HourQuote *hourQuote_from_js_new(Js *js);

/*--*/

/// Sets data for a new day
///   nick: Nick to set.
///   pfv_null: Portfolio data or NULL if nick is not bought.
///   lastq: Last historic close.
///   signal: (> 0) -> Suport (buying).
///           (< 0) -> Resitence (selling).
///           (= 0) -> Not operation.
void nick_new(char *nick, PfValue *pfv, double lastq, double signal);

/// Update nick quote
void nick_update(char *nick, double quote);

#endif
