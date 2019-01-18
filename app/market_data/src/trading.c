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
  char *conf = path_cat_new(FLEAS, "conf.db", NULL);
  Js *js = (Js *)file_read_new(conf);
  free(conf);

  // Map[Js]
  Map *m = js_ro_new(js);
  free(js);

  Js *max_days = map_get_null(m, "max_days");
  if (!max_days) FAIL("max_days not found");
  Js *min_days = map_get_null(m, "min_days");
  if (!min_days) FAIL("min_days not found");
  Js *max_strip = map_get_null(m, "max_strip");
  if (!max_strip) FAIL("max_strip not found");

  int max = js_ri(max_days);
  int min = js_ri(min_days);
  double strip = js_rd(max_strip);

  map_free(m);

  char *bests_dir = path_cat_new(FLEAS, "bests", NULL);
  // Arr[char]
  Arr *fs = file_dir_new(bests_dir);
  int size = arr_size(fs);
  if (!size) FAIL("fs is empty");
  free(bests_dir);

  arr_sort(fs, (FGREATER)str_greater);
  char *fps = path_cat_new(FLEAS, "bests", arr_get(fs, size - 1), NULL);
  arr_free(fs);

  Js *ps_js = (Js *)file_read_new(fps);
  free(fps);

  // Arr[Js]
  Arr *a0 = js_ra_new(ps_js);
  free(ps_js);
  size = arr_size(a0);
  if (!size) {
    return params_new(50, 0.05, 0.05);
  }

  // Arr[Js]
  Arr *a1 = js_ra_new(arr_get(a0, size - 1));
  arr_free(a0);

  // Arr[Js]
  Arr *a2 = js_ra_new(arr_get(a1, 1));
  arr_free(a1);

  // Arr[Js]
  Arr *a3 = js_ra_new(arr_get(a2, 0));
  arr_free(a2);

  // Arr[Js]
  Arr *a4 = js_ra_new(arr_get(a3, 1));
  arr_free(a3);

  Params *r = params_new(
    min + (int)((max - min) * js_rd(arr_get(a4, 0))),
    strip * js_rd(arr_get(a4, 1)),
    strip * js_rd(arr_get(a4, 2))
  );
  arr_free(a4);

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
    if (after >= size - 1) {
      break;
    }

    double cl = darr_get(closes, after);
    double bf = darr_get(closes, before);
    if (cl > 0) {
      if (isBuying) {
        if ((cl - ref) / ref > bs) {
          isBuying = 0;
          ref = bf;
        }
      } else {
        if ((ref - cl) / ref > ss) {
          isBuying = 1;
          ref = bf;
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
