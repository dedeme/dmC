// Copyright 24-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Rs.h"

/* .
Rs: serial
  #Freed by this.
  date: char *
  #Freed by this.
  flea: Flea *
  #Actual parameters (not gen parameters). Freed by this.
  params: Darr *
  #Money. Calculus in market condition.
  assets: double::_d9
  #Ratio. Exhaustive calculus
  @profits_avg: double::_d9
  #Normalized from 0 (all values equals) to (0,5) max deviation.
  @profits_mdv: double::_d9
  #= profits_avg * (1 - profits_mdv)
  @profits_sel: double::_d9
  buys: int
  sells: int
*/

/*--*/

struct rs_Rs {
  char *date;
  Flea *flea;
  Darr *params;
  double assets;
  double profits_avg;
  double profits_mdv;
  double profits_sel;
  int buys;
  int sells;
};

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
) {
  Rs *this = malloc(sizeof(Rs));
  this->date = date;
  this->flea = flea;
  this->params = params;
  this->assets = assets;
  this->profits_avg = profits_avg;
  this->profits_mdv = profits_mdv;
  this->profits_sel = profits_sel;
  this->buys = buys;
  this->sells = sells;
  return this;
}

void rs_free(Rs *this) {
  free(this->date);
  flea_free(this->flea);
  darr_free(this->params);
  free(this);
};

char *rs_date(Rs *this) {
  return this->date;
}

Flea *rs_flea(Rs *this) {
  return this->flea;
}

Darr *rs_params(Rs *this) {
  return this->params;
}

double rs_assets(Rs *this) {
  return this->assets;
}

double rs_profits_avg(Rs *this) {
  return this->profits_avg;
}

double rs_profits_mdv(Rs *this) {
  return this->profits_mdv;
}

double rs_profits_sel(Rs *this) {
  return this->profits_sel;
}

int rs_buys(Rs *this) {
  return this->buys;
}

int rs_sells(Rs *this) {
  return this->sells;
}

void rs_set_profits_avg(Rs *this, double v) {
  this->profits_avg = v;
}

void rs_set_profits_mdv(Rs *this, double v) {
  this->profits_mdv = v;
}

void rs_set_profits_sel(Rs *this, double v) {
  this->profits_sel = v;
}

Js *rs_to_js_new(Rs *this) {
  // Arr[Js]
  Arr *a = arr_new(free);
  arr_push(a, js_ws_new(this->date));
  arr_push(a, flea_to_js_new(this->flea));
  arr_push(a, darr_to_js_new(this->params));
  arr_push(a, js_wd_new(this->assets, 9));
  arr_push(a, js_wd_new(this->profits_avg, 9));
  arr_push(a, js_wd_new(this->profits_mdv, 9));
  arr_push(a, js_wd_new(this->profits_sel, 9));
  arr_push(a, js_wi_new(this->buys));
  arr_push(a, js_wi_new(this->sells));
  Js *r = js_wa_new(a);
  arr_free(a);
  return r;
}

Rs *rs_from_js_new(Js *js) {
  // Arr[Js]
  Arr *a = js_ra_new(js);
  int i = 0;
  char *date = js_rs_new(arr_get(a, i++));
  Flea *flea = flea_from_js_new(arr_get(a, i++));
  Darr *params = darr_from_js_new(arr_get(a, i++));
  double assets = js_rd(arr_get(a, i++));
  double profits_avg = js_rd(arr_get(a, i++));
  double profits_mdv = js_rd(arr_get(a, i++));
  double profits_sel = js_rd(arr_get(a, i++));
  int buys = js_ri(arr_get(a, i++));
  int sells = js_ri(arr_get(a, i++));
  arr_free(a);
  return rs_new(date, flea, params, assets, profits_avg, profits_mdv, profits_sel, buys, sells);
}

/*--*/
