// Copyright 12-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "pgs/settings/nicks/nkeditor.h"
#include "kut/rs.h"
#include "kut/js.h"
#include "mkt/qs.h"
#include "mkt/quote.h"
#include "data/cts.h"
#include "data/qsvs.h"
#include "db/quotesDb.h"
#include "db/svCodesTb.h"
#include "db/log.h"
#include "qspr.h"
#include "pgs.h"

// mrq is Map<char>
void nkeditor_process (Map *mrq) {
  char *rq = pgs_rs(mrq, "rq");
  if (str_eq(rq, "idata")) {
    char *main_nick = pgs_rs(mrq, "mainNick");
    char *nick = pgs_rs(mrq, "nick");
    int ok = TRUE;
    // <JS>
    Arr *quotes = arr_new();
    int manuals = 0;
    // <JS>
    Arr *mquotes = arr_new();
    // <JS>
    Arr *sIdCodes = arr_new();
    // <Qs>
    Rs *mqs_rs = quotesDb_read_qs(main_nick);
    Qs *mqs = rs_get(mqs_rs);
    if (mqs) {
      mquotes = arr_map(qs_quotes(mqs), (FMAP)quote_to_js);
    } else {
      log_error(str_f("Fail reading %s: %s", main_nick, rs_error(mqs_rs)));
      ok = FALSE;
    }

    if (ok) {
      // <Qs>
      Rs *qs_rs = quotesDb_read_qs(nick);
      Qs *qs = rs_get(qs_rs);
      if (qs) {
        // <Quote>
        Arr *qs2 = qs_quotes(qs);
        quotes = arr_map(qs2, (FMAP)quote_to_js);
        EACH(qs2, Quote, q) {
          if (q->force) ++manuals;
        }_EACH
      } else {
        log_error(str_f("Fail reading %s: %s", nick, rs_error(qs_rs)));
        ok = FALSE;
      }
    }

    if (ok) {
      EACH(qsvs_list(), Qsv, sv) {
        char *id = qsv_id(sv);
        // <char>
        Map *codes = svCodesTb_read(id);
        char *code = opt_get(map_get(codes, nick));
        if (code) {
          arr_push(sIdCodes, js_wa(arr_new_from(
            js_ws(id), js_wb(qsv_with_historic(sv)),  js_ws(code), NULL
          )));
        } else {
          log_error(str_f("Fail reading code of %s in server %s", nick, id));
          ok = FALSE;
        }
      }_EACH
    }

    //<char>
    Map *rp = map_new();
    map_add(rp, "ok", js_wb(ok));
    map_add(rp, "quotes", js_wa(quotes));
    map_add(rp, "manuals", js_wi(manuals));
    map_add(rp, "mquotes", js_wa(mquotes));
    map_add(rp, "sIdCodes", js_wa(sIdCodes));
    pgs_print(rp);
  } else if (str_eq(rq, "updateCode")) {
    char *server = pgs_rs(mrq, "server");
    char *nick = pgs_rs(mrq, "nick");
    char *code = pgs_rs(mrq, "code");
    // <char>
    Map *nk_cds = svCodesTb_read(server);
    map_put(nk_cds, nick, code);
    svCodesTb_write(server, nk_cds);
    pgs_print_empty();
  } else if (str_eq(rq, "serverTest")) {
    char *server = pgs_rs(mrq, "server");
    char *nick = pgs_rs(mrq, "nick");
    char *code = pgs_rs(mrq, "code");
    int withErrors = FALSE;
    int withWarnings = FALSE;
      //--
      int ffind (Qsv *sv) { return str_eq(qsv_id(sv), server); }
    Qsv *sv = opt_get(arr_find(qsvs_list(), (FPRED)ffind));
    if (sv) {
      //<char>
      Rs *html_rs = qsv_download_historic(sv, code);
      char *html = rs_get(html_rs);
      if (html) {
        // Tp<Opt<Qs>>, Arr<char>>
        Tp *tp = qsv_read_historic(sv, html);
        Qs *qs = opt_get(tp_e1(tp));
        if (qs) {
          if (arr_size(tp_e2(tp))) {
            EACH(tp_e2(tp), char, err){
              log_error(str_f("(Test in %s:%s) %s", server, nick, err));
            }_EACH
            withWarnings = TRUE;
          }
        } else {
          EACH(tp_e2(tp), char, err){
            log_error(str_f("(Test in %s:%s) %s", server, nick, err));
          }_EACH
          withWarnings = TRUE;
          withErrors = TRUE;
        }
      } else {
        log_error(str_f("(Test in %s:%s) %s", server, nick, rs_error(html_rs)));
        withErrors = TRUE;
      }
    } else {
      log_error(str_f("Server '%s' not found", server));
      withErrors = TRUE;
    }
    //<char>
    Map *rp = map_new();
    map_add(rp, "withErrors", js_wb(withErrors));
    map_add(rp, "withWarnings", js_wb(withWarnings));
    pgs_print(rp);
  } else if (str_eq(rq, "download")) {

    //<char>
    Map *rp = map_new();
    map_add(rp, "result", js_ws("a"));
    pgs_print(rp);

  } else if (str_eq(rq, "test")) {
    char *main_nick = pgs_rs(mrq, "mainNick");
    char *nick = pgs_rs(mrq, "nick");
    char *qs = pgs_rs(mrq, "qs");

    char *prefix = str_f("Fail testing %s", nick);
    QsprRs *rs = str_eq(nick, main_nick)
      ? qspr_correct_str(prefix, qs)
      : qspr_correct_str2(prefix, qs, main_nick)
    ;

    //<char>
    Map *rp = map_new();
    map_add(rp, "result", js_ws(
      rs->withError
        ? "error"
        : rs->withWarnings
          ? "warnings"
          : ""
    ));
    pgs_print(rp);
  } else if (str_eq(rq, "qModify")) {
    char *main_nick = pgs_rs(mrq, "mainNick");
    char *nick = pgs_rs(mrq, "nick");
    char *qs = pgs_rs(mrq, "qs");

    char *prefix = str_f("Fail modifying %s", nick);
    QsprRs *rs = str_eq(nick, main_nick)
      ? qspr_correct_str(prefix, qs)
      : qspr_correct_str2(prefix, qs, main_nick)
    ;

    int ok = !rs->withError && !rs->withWarnings;

    if (ok) quotesDb_write(nick, qs);

    //<char>
    Map *rp = map_new();
    map_add(rp, "ok", js_wb(ok));
    pgs_print(rp);
  } else if (str_eq(rq, "getQuotes")) {
    char *nick = pgs_rs(mrq, "nick");
    int ok = TRUE;
    // <Qs>
    Rs *qs_rs = quotesDb_read_qs(nick);
    Qs *qs = rs_get(qs_rs);
    if (!qs) {
      log_error(str_f("Fail reading %s: %s", nick, rs_error(qs_rs)));
      ok = FALSE;
    }

    //<char>
    Map *rp = map_new();
    map_add(rp, "ok", js_wb(ok));
    map_add(rp, "quotes", ok
      ? js_wa(arr_map(qs_quotes(qs), (FMAP)quote_to_js))
      : js_wa(arr_new())
    );
    pgs_print(rp);
  } else {
    EXC_GENERIC(str_f(
      "Value '%s' not found for 'rq' in '%s'", rq, js_wo(mrq)
    ));
  }
}
