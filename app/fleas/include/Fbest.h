// Copyright 21-Nov-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Data of selected flea

#ifndef FBEST_H
  #define FBEST_H

#include "dmc/std.h"
#include "Fresults.h"

/*--*/

///
typedef struct fbest_Fbest Fbest;

///
Fbest *fbest_new(
  char *date,
  Fresults *fresults,
  double average,
  double cv
);

///
void fbest_free(Fbest *this);

///
char *fbest_date(Fbest *this);

///
Fresults *fbest_fresults(Fbest *this);

///
double fbest_average(Fbest *this);

///
double fbest_cv(Fbest *this);

///
Js *fbest_to_js_new(Fbest *this);

///
Fbest *fbest_from_js_new(Js *js);

/*--*/

Fbest *fbest_copy_new(Fbest *this);

#endif
