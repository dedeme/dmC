// Copyright 20-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/moddec.h"
#include "dmc/date.h"
#include "fails.h"
#include "Machine.h"
#include "primitives/modlong.h"

struct timeval *totimeval (Machine *m, Token *tk) {
  if (token_type(tk) != token_LIST) fails_type_in(m, token_LIST, tk);
  // Arr<Token>
  Arr *a = token_list(tk);
  if (arr_size(a) != 2) fails_size_list(m, a, 2);
  struct timeval *tv = MALLOC(struct timeval);
  tv->tv_sec = modlong_to_long(m, *arr_start(a));
  tv->tv_usec = modlong_to_long(m, *(arr_start(a) + 1));
  return tv;
}

Token *fromtimeval (struct timeval *tv) {
  return token_new_list(0, arr_new_from(
    modlong_from_long(tv->tv_sec), modlong_from_long(tv->tv_usec), NULL
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

static void add (Machine *m) {
  int millis = token_int(machine_pop_exc(m, token_INT));
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

// Resturns Map<primitives_Fn>
Map *moddec_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "now", now); // [] - LIST   (() - list (LONG, LONG)
  map_put(r, "-", sub); // [LIST - LIST] - LIST   lists ar list (LONG, LONG)
  map_put(r, "add", add); // [LIST- INT] - LIST   lists ar list (LONG, LONG)
  map_put(r, "df", df); // [LIST - LIST] - INT   lists ar list (LONG, LONG)

  return r;
}



