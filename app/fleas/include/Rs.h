// Copyright 24-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef RS_H
  #define RS_H

#include "dmc/std.h"
#include "dmc/Darr.h"
#include "Flea.h"

/*--*/

///
typedef struct rs_Rs Rs;

///
Rs *rs_new(
  char *date,
  Flea *flea,
  Darr *params,
  double assets,
  double profits_avg,
  double profits_mdv,
  double profits_sel,
  int buys,
  int sells
);

///
void rs_free(Rs *this);

/// Freed by this.
char *rs_date(Rs *this);

/// Freed by this.
Flea *rs_flea(Rs *this);

/// Actual parameters (not gen parameters). Freed by this.
Darr *rs_params(Rs *this);

/// Money. Calculus in market condition.
double rs_assets(Rs *this);

/// Ratio. Exhaustive calculus
double rs_profits_avg(Rs *this);

/// Normalized from 0 (all values equals) to (0,5) max deviation.
double rs_profits_mdv(Rs *this);

/// = profits_avg * (1 - profits_mdv)
double rs_profits_sel(Rs *this);

///
int rs_buys(Rs *this);

///
int rs_sells(Rs *this);

/// Ratio. Exhaustive calculus
void rs_set_profits_avg(Rs *this, double v);

/// Normalized from 0 (all values equals) to (0,5) max deviation.
void rs_set_profits_mdv(Rs *this, double v);

/// = profits_avg * (1 - profits_mdv)
void rs_set_profits_sel(Rs *this, double v);

///
Js *rs_to_js_new(Rs *this);

///
Rs *rs_from_js_new(Js *js);

/*--*/

#endif
