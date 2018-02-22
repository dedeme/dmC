// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Day quote

#ifndef QUOTE_H
  # define QUOTE_H

#include "Json.h"

///
typedef struct quote_Quote Quote;

/// quote_from_str reads a quote from BolsaData in format:
///   date:open:close:max:min:vol:[true | false]
Quote *quote_from_str(char *q);

///
double quote_open(Quote *this);

///
double quote_close(Quote *this);

///
double quote_max(Quote *this);

///
double quote_min(Quote *this);

///
size_t quote_vol(Quote *this);

///
Json *quote_serialize(Quote *q);

///
Quote *quote_restore(Json *serial);

#endif


