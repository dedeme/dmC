// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Results tracer

#ifndef TRACE_H
  # define TRACE_H

#include <stdlib.h>
#include <dm/Json.h>
#include "market/Quote.h"
#include "Portfolio.h"

///
typedef struct trace_Trace Trace;

/// trace_new creates a new Trace
Trace *trace_new(
  char *date,
	Quote *quote,
	double beforeCash,
	Portfolio *beforePortfolio,
	char *nick,
	double cash, // if order is sell its value is 0
	uint stocks, // if order is buy its value is 0
	double afterCash,
	Portfolio *afterPortfolio,
	Json *family_data
);

///
Trace *trace_copy(Trace *this);

/// trace_serialize serializes 'this' only to be saved, therefor there is not
/// the corresponding trace_restore.
Json *trace_serialize(Trace *this, Nicks *nicks);

#endif


