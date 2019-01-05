// Copyright 28-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Fresults.h"
/* .
Fresults: serial
  flea: Flea *
  assets: double:: _d2
  buys: int
  sells: int
*/

/*--*/

struct fresults_Fresults {
  Flea *flea;
  double assets;
  int buys;
  int sells;
};

Fresults *fresults_new(
  Flea *flea,
  double assets,
  int buys,
  int sells
) {
  Fresults *this = malloc(sizeof(Fresults));
  this->flea = flea;
  this->assets = assets;
  this->buys = buys;
  this->sells = sells;
  return this;
}

void fresults_free(Fresults *this) {
  flea_free(this->flea);
  free(this);
};

Flea *fresults_flea(Fresults *this) {
  return this->flea;
}

double fresults_assets(Fresults *this) {
  return this->assets;
}

int fresults_buys(Fresults *this) {
  return this->buys;
}

int fresults_sells(Fresults *this) {
  return this->sells;
}

Js *fresults_to_js_new(Fresults *this) {
  // Arr[Js]
  Arr *a = arr_new(free);
  arr_push(a, flea_to_js_new(this->flea));
  arr_push(a, js_wd_new(this->assets, 2));
  arr_push(a, js_wi_new(this->buys));
  arr_push(a, js_wi_new(this->sells));
  Js *r = js_wa_new(a);
  arr_free(a);
  return r;
}

Fresults *fresults_from_js_new(Js *js) {
  // Arr[Js]
  Arr *a = js_ra_new(js);
  int i = 0;
  Flea *flea = flea_from_js_new(arr_get(a, i++));
  double assets = js_rd(arr_get(a, i++));
  int buys = js_ri(arr_get(a, i++));
  int sells = js_ri(arr_get(a, i++));
  arr_free(a);
  return fresults_new(flea, assets, buys, sells);
}

/*--*/

Fresults *fresults_copy_new(Fresults *this) {
  return fresults_new(
    flea_copy_new(this->flea), this->assets, this->buys, this->sells
  );
}
