// Copyright 05-06-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "db/cosTb.h"
#include "kut/DEFS.h"
#include "data/co.h"
#include "db/quotesDb.h"
#include "db.h"

static char *fpath = "cos.tb";

void cosTb_init (void) {
  db_write(fpath, cosT_to_js(cosT_new("", arr_new())));
}

/// Returns companies in quotes data base.
CosT *cosTb_read (void) {
  // <char>
  Arr *nicks = quotesDb_nicks();

  int changed = FALSE;
  CosT *cost = cosT_from_js(db_read(fpath));
  // <Co>
  Arr *new_cos = arr_new();
  EACH(cost->cos, Co, old_c) {
      //--
      int any (char *nk) { return str_eq(nk, old_c->nick); }
    if (arr_any(nicks, (FPRED)any)) arr_push(new_cos, old_c);
    else changed = TRUE;
  }_EACH
  EACH(nicks, char, nk) {
      //--
      int any (Co *co) { return str_eq(co->nick, nk); }
    if (!arr_any(new_cos, (FPRED)any)) {
      arr_push(new_cos, co_new(nk, FALSE));
      changed = TRUE;
    }
  }_EACH
  char *main = cost->main;
    //--
    int any (char *nk) { return str_eq(nk, main); }
  if (!arr_any(nicks, (FPRED)any) && arr_size(nicks)) {
    main = arr_get(nicks, 0);
    changed = TRUE;
  }

  CosT *r = cosT_new(main, new_cos);
  if (changed) db_write(fpath, cosT_to_js(r));
  return r;
}

void cosTb_select (char *nick, int value) {
  CosT *cost = cosTb_read();
  // <Co>
  Arr *new_cos = arr_new();
  EACH(cost->cos, Co, c) {
    arr_push(new_cos, str_eq(c->nick, nick) ? co_new(nick, value) : c);
  }_EACH
  db_write(fpath, cosT_to_js(cosT_new(cost->main, new_cos)));
}

void cosTb_setMain (char *nick) {
  CosT *cost = cosTb_read();
  db_write(fpath, cosT_to_js(cosT_new(nick, cost->cos)));
}
