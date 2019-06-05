// Copyright 15-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "trading.h"
#include "DEFS.h"
#include "io.h"

struct trading_Params {
  double step;
};

#define Params struct trading_Params

static Params *params_new(double step) {
  Params *this = malloc(sizeof(Params));
  this->step = step;
  return this;
}

static void params_free(Params *this) {
  free(this);
};

static Params *params_read_new(void) {
  char *path = path_cat_new(MARKET, "baseParams.db", NULL);
  Js *js = (Js *)file_read_new(path);
  free(path);
  // Arr[Js]
  Arr *values = js_ra_new(js);
  Params *r = params_new(
    js_rd(arr_get(values, 0))
  );
  free(js);
  free(values);
  return r;
}

static Darr *closes_new(char *nick) {
  Darr *r = darr_new();

  char *path = str_f_new("%s/quotes/%s.db", QUOTES, nick);
  if (!file_exists(path)) FAIL(str_f_new("'%s' not found in Quotes", nick));
  char *data = file_read_new(path);
  free(path);
  // Arr[char]
  Arr *rows = str_csplit_new(data, '\n');
  free(data);
  EACH(rows, char, row)
    // Arr[char]
    Arr *fields = str_csplit_new(row, ':');
    double c = atof(arr_get(fields, 2));
    if (c > 0) {
      darr_push(r, c);
    }
    arr_free(fields);
  _EACH
  arr_free(rows);

  darr_reverse(r);
  return r;
}

static double force(char *nick) {
  //return str_eq(nick, "SAN") ? 4.4855
  //  : -2
  return -2;
}

static double sup_res(Darr *closes, Params *ps, double force) {
  double step = ps->step;
  int is_to_sell = 1;
  double ref = darr_get(closes, 0) * 0.95;
  double mm = darr_get(closes, 0);
  double pmm1 = mm;
  double pmm2 = mm;
  double coq = mm;

  DEACH(closes, cl)
    if (is_to_sell) {
      ref = ref + (cl - ref) * step;
      if (cl <= ref) {
        ref = cl > coq || mm > pmm2 ? mm : pmm2;
        pmm2 = pmm1;
        pmm1 = mm;
        mm = cl;
        coq = cl;
        is_to_sell = 0;
      } else {
        mm = cl > mm ? cl : mm;
      }
    } else {
      ref = ref - (ref - cl) * step;
      if (cl >= ref || cl == force) {
        ref = cl < coq || mm < pmm2 ? mm : pmm2;
        pmm2 = pmm1;
        pmm1 = mm;
        mm = cl;
        coq = cl;
        is_to_sell = 1;
      } else {
        mm = cl < mm ? cl : mm;
      }
    }
  _EACH

  return is_to_sell ? -ref : ref;
}

// Nicks is Arr[char]
void trading_read_new(Darr **last_qs_new, Darr **signals_new, Arr *nicks) {
  Params *ps = params_read_new();

  Darr *qs = darr_new();
  Darr *ss = darr_new();

  EACH(nicks, char, nick)
    Darr *cls = closes_new(nick);

    darr_push(qs, darr_get(cls, darr_size(cls) - 1));
    darr_push(ss, sup_res(cls, ps, force(nick)));
    darr_free(cls);
  _EACH

  params_free(ps);

  *last_qs_new = qs;
  *signals_new = ss;
}
