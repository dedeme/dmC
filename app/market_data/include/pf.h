// Copyright 15-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Portfolio.

#ifndef PF_H
  #define PF_H

#include "dmc/std.h"

/*--*/

///
typedef struct pfValue_PfValue PfValue;

///
void pfValue_free(PfValue *this);

///
int pfValue_stocks(PfValue *this);

///
double pfValue_price(PfValue *this);

/*--*/

/// Reads porfolio.<p>Returns a Map[PfValue]
Map *pf_read_new(void);

#endif
