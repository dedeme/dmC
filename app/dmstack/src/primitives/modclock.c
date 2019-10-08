// Copyright 20-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modclock.h"
#include "dmc/date.h"
#include "fails.h"
#include "tk.h"

static struct timeval *totimeval (Machine *m, Token *tk) {
  // Arr<Token>
  Arr *a = tk_list(m, tk);
  if (arr_size(a) != 2) fails_size_list(m, a, 2);

  struct timeval *tv = MALLOC(struct timeval);
  tv->tv_sec = tk_int(m, *arr_start(a));
  tv->tv_usec = tk_int(m, *(arr_start(a)+ 1));
  return tv;
}

static Token *fromtimeval (struct timeval *tv) {
  return token_new_list(0, arr_new_from(
    token_new_int(0, tv->tv_sec), token_new_int(0, tv->tv_usec), NULL
  ));
}

static void now (Machine *m) {
  machine_push(m, fromtimeval(dateTm_now()));
}

static void sub (Machine *m) {
  Token *v2 = machine_pop(m);
  Token *v1 = machine_pop(m);
  machine_push(m, fromtimeval(
    dateTm_tdf(totimeval(m, v1), totimeval(m, v2))
  ));
}

static void sadd (Machine *m) {
  Int millis = tk_pop_int(m);
  Token *v = machine_pop(m);
  machine_push(m, fromtimeval(
    dateTm_add(totimeval(m, v), millis)
  ));
}

static void df (Machine *m) {
  Token *v2 = machine_pop(m);
  Token *v1 = machine_pop(m);
  machine_push(m, token_new_int(
    0, dateTm_df(totimeval(m, v1), totimeval(m, v2))
  ));
}

Pmodule *modclock_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("now", now); // [] - LIST   (() - list (LONG, LONG)
  add("-", sub); // [LIST - LIST] - LIST   lists ar list (LONG, LONG)
  add("add", sadd); // [LIST- INT] - LIST   lists ar list (LONG, LONG)
  add("df", df); // [LIST - LIST] - INT   lists ar list (LONG, LONG)

  return r;
}


