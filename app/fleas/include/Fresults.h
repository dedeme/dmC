// Copyright 28-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Flea results of a cycle

#ifndef FRESULTS_H
  #define FRESULTS_H

#include "dmc/std.h"
#include "Flea.h"

/*--*/

///
typedef struct fresults_Fresults Fresults;

///
Fresults *fresults_new(
  Flea *flea,
  double assets,
  int buys,
  int sells
);

///
void fresults_free(Fresults *this);

///
Flea *fresults_flea(Fresults *this);

///
double fresults_assets(Fresults *this);

///
int fresults_buys(Fresults *this);

///
int fresults_sells(Fresults *this);

///
Js *fresults_to_js_new(Fresults *this);

///
Fresults *fresults_from_js_new(Js *js);

/*--*/

///
Fresults *fresults_copy_new(Fresults *this);

#endif
