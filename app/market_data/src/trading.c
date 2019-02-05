// Copyright 15-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "trading.h"
#include "DEFS.h"

struct trading_Params {
  int days;
  double bs;
  double ss;
};

#define Params struct trading_Params

static Params *params_new(int days, double bs, double ss) {
  Params *this = malloc(sizeof(Params));
  this->days = days;
  this->bs = bs;
  this->ss = ss;
  return this;
}

static void params_free(Params *this) {
  free(this);
};

static Params *params_read_default_new(void) {
  char *path = path_cat_new(MARKET, "baseParams.db", NULL);
  Js *js = (Js *)file_read_new(path);
  free(path);
  // Arr[Js]
  Arr *values = js_ra_new(js);
  Params *r = params_new(
    js_ri(arr_get(values, 0)),
    js_rd(arr_get(values, 1)),
    js_rd(arr_get(values, 2))
  );
  free(js);
  free(values);
  return r;
}

// Returns Map[Params]
static Map *params_read_new(void) {
  char *path = path_cat_new(MARKET, "params.db", NULL);
  Js *js = (Js *)file_read_new(path);
  free(path);
  // Arr[Js]
  Arr *ps = js_ra_new(js);
  free(js);

  // Map[Params]
  Map *r = map_new((FPROC)params_free);

  EACH(ps, Js, js)
    // Arr[Js]
    Arr *a = js_ra_new(js);
    char *nick = js_rs_new(arr_get(a, 0));
    // Arr[js]
    Arr *values = js_ra_new(arr_get(a, 1));

    map_put(r, nick, params_new(
      js_ri(arr_get(values, 0)),
      js_rd(arr_get(values, 1)),
      js_rd(arr_get(values, 2))
    ));

    arr_free(values);
    free(nick);
    arr_free(a);
  _EACH

  arr_free(ps);
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
    darr_push(r, atof(arr_get(fields, 2)));
    arr_free(fields);
  _EACH
  arr_free(rows);

  darr_reverse(r);
  return r;
}

static double sup_res(Darr *closes, Params *ps) {
  double bs = ps->bs;
  double ss = ps->ss;
  int before = 0;
  int after = ps->days - 1;
  double ref = -1;
  int isBuying = 0;
  int size = darr_size(closes);

  for (;;) {
    double begin = darr_get(closes, before);
    if (ref <= 0) {
      if (begin > 0) {
        ref = begin;
      } else {
        ++before;
        ++after;
        if (after >= size) break; else continue;
      }
    } else {
      if (begin > 0) {
        if (isBuying) {
          if (begin < ref) ref = begin;
        } else {
          if (begin > ref) ref = begin;
        }
      }
    }

    ++before;
    ++after;
    if (after >= size) {
      break;
    }

    double cl = darr_get(closes, after);
    if (cl > 0) {
      if (isBuying) {
        if ((cl - ref) / ref > bs) {
          isBuying = 0;
          ref = darr_get(closes, before);
        }
      } else {
        if ((ref - cl) / ref > ss) {
          isBuying = 1;
          ref = darr_get(closes, before);
        }
      }
    }
  }

  return ref
    ? isBuying ? ref * (1 + bs) : -(ref * (1 - ss))
    : 0
  ;
}

void trading_read_new(Darr **last_qs_new, Darr **signals_new) {
  Params *def = params_read_default_new();
  // Map[params]
  Map *ps = params_read_new();

  Darr *qs = darr_new();
  Darr *ss = darr_new();

  char *nicks[] = NICKS;
  char **p = nicks;
  while (*p) {
    char *nick = *p++;
    Darr *cls = closes_new(nick);
    Params *p = map_get_null(ps, nick);
    if (!p) p = def;

    darr_push(qs, darr_get(cls, darr_size(cls) - 1));
    darr_push(ss, sup_res(cls, p));
    darr_free(cls);
  }

  map_free(ps);
  params_free(def);

  *last_qs_new = qs;
  *signals_new = ss;
}
