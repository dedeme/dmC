// Copyright 05-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "pgs/settings/nicks/nicks.h"
#include "kut/rs.h"
#include "kut/dec.h"
#include "mkt/qs.h"
#include "data/cts.h"
#include "db/quotesDb.h"
#include "db/cosTb.h"
#include "db/log.h"
#include "pgs.h"
#include "data/cosT.h"
#include "data/co.h"

// mrq is Map<char>
void nicks_process (Map *mrq) {
  char *rq = pgs_rs(mrq, "rq");
  if (str_eq(rq, "idata")) {
    CosT *cost = cosTb_read();
    // <char>
    Map *volumes = map_new();
    EACH(cost->cos, Co, c) {
      //<Qs>
      Rs *qs_rs = quotesDb_read_qs(c->nick);
      Qs *qs = rs_get(qs_rs);
      if (qs) {
        map_add(volumes, c->nick, js_wf(qs_volume_avg(qs, QUOTES_VOLUME), 4));
      } else {
        map_add(volumes, c->nick, "0");
      }
    }_EACH
    //<char>
    Map *rp = map_new();
    map_add(rp, "main", js_ws(cost->main));
    map_add(rp, "cos", arr_to_js(cost->cos, (FTO)co_to_js));
    map_add(rp, "vol", js_wo(volumes));
    pgs_print(rp);
  } else if (str_eq(rq, "setMain")) {
    cosTb_setMain(pgs_rs(mrq, "nick"));
    pgs_print_empty();
  } else if (str_eq(rq, "select")) {
    cosTb_select(pgs_rs(mrq, "nick"), pgs_rb(mrq, "value"));
    pgs_print_empty();
  } else if (str_eq(rq, "test")) {
    char *nk = pgs_rs(mrq, "nick");
    int with_errors = FALSE;
    int with_warnings = FALSE;
    // <Qs>
    Rs *qs_rs = quotesDb_read_qs(nk);
    Qs *qs = rs_get(qs_rs);
    if (qs) {
      // <Qs, Arr<char>>
      Tp *tp = qs_correct(qs);
      // <char>
      Arr *errs = tp_e2(tp);
      Qs *mqs = rs_get(quotesDb_read_qs(cosTb_read()->main));
      if (mqs) {
        tp = qs_correct_dates(mqs, tp_e1(tp));
        arr_cat(errs, tp_e2(tp));
      }
      if (arr_size(errs)){
        with_warnings = TRUE;
        log_error(str_f(
          "Warnings testing %s (%s and %d more)",
          nk, arr_get(errs, 0), arr_size(errs) - 1
        ));
      }
    } else {
      with_errors = TRUE;
      log_error(str_f("Error testing %s (%s)", nk, rs_error(qs_rs)));
    }
    //<char>
    Map *rp = map_new();
    map_add(rp, "withErrors", js_wb(with_errors));
    map_add(rp, "withWarnings", js_wb(with_warnings));
    pgs_print(rp);
  } else {
    EXC_GENERIC(str_f(
      "Value '%s' not found for 'rq' in '%s'", rq, js_wo(mrq)
    ));
  }
}
