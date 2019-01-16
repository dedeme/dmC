// Copyright 15-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "pf.h"
#include "DEFS.h"

/* .
-PfValue
  stocks: int
  price: double :: _d4
*/

/*--*/

struct pfValue_PfValue {
  int stocks;
  double price;
};

PfValue *_pfValue_new(int stocks, double price) {
  PfValue *this = malloc(sizeof(PfValue));
  this->stocks = stocks;
  this->price = price;
  return this;
}

void pfValue_free(PfValue *this) {
  free(this);
};

int pfValue_stocks(PfValue *this) {
  return this->stocks;
}

double pfValue_price(PfValue *this) {
  return this->price;
}

/*--*/

/// Reads porfolio. Returns a Map[PfValue]
Map *pf_read_new(void) {
  char *dir = path_cat_new(MARKET, "Diaries", NULL);
  // Arr[char]
  Arr *ds = file_dir_new(dir);
  arr_sort(ds, (FGREATER)str_greater);

  // Map[PfValue]
  Map *r = map_new((FPROC)pfValue_free);

  EACH(ds, char, year)
    char *path = path_cat_new(dir, year, NULL);
    Js *js0 = (Js *)file_read_new(path);
    free(path);
    // Arr[Js]
    Arr *a0 = js_ra_new(js0);
    free(js0);
    // Arr[Js]
    Arr *a1 = js_ra_new(arr_get(a0, 1));
    arr_reverse(a1);
    arr_free(a0);

    EACH(a1, Js, row_js)
      // Arr[Js]
      Arr *row = js_ra_new(row_js);
      char *type = js_rs_new(arr_get(row, 0));
      if (str_eq(type, "bu")) {
        char *nick = js_rs_new(arr_get(row, 3));
        int stocks = js_ri(arr_get(row, 4));
        double price = js_rd(arr_get(row, 5));
        PfValue *v = map_get_null(r, nick);
        if (v) {
          int tt = v->stocks + stocks;
          double new_price = (v->stocks * v->price + stocks * price) / tt;
          v->stocks = tt;
          v->price = new_price;
        } else {
          v = _pfValue_new(stocks, price);
          map_put(r, nick, v);
        }
        free(nick);
      } else if (str_eq(type, "se")) {
        char *nick = js_rs_new(arr_get(row, 3));
        int stocks = js_ri(arr_get(row, 4));
        PfValue *v = map_get_null(r, nick);
        if (v) {
          int dif = v->stocks - stocks;
          if (dif < 0) dif = 0;
          if (dif == 0) {
            map_remove(r, nick);
          } else {
            v->stocks = dif;
          }
        }
        free(nick);
      }
      arr_free(row);
      free(type);
    _EACH

    arr_free(a1);
  _EACH

  arr_free(ds);
  free(dir);
  return r;
}

