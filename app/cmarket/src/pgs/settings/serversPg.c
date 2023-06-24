// Copyright 01-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "pgs/settings/serversPg.h"
#include "kut/DEFS.h"
#include "kut/rs.h"
#include "mkt/qs.h"
#include "db/log.h"
#include "data/qsvs.h"
#include "pgs.h"
#include "db/svCodesTb.h"

// mrq is Map<char>
void serversPg_process (Map *mrq) {
  char *rq = pgs_rs(mrq, "rq");

  if (str_eq(rq, "idata")) {
    char *selected = pgs_rs(mrq, "selected");

    // <svData>
    Arr *svs = arr_new();
    EACH(qsvs_list(), Qsv, sv) {
      char *id = qsv_id(sv);
      arr_push(
        svs,
        js_wa(arr_new_from(
          js_ws(id),
          js_ws(qsv_name(sv)),
          js_wb(qsv_with_diary(sv)),
          js_wb(qsv_with_historic(sv)),
          js_wo(map_new()),
          NULL
        ))
      );
    }_EACH

    // <s>
    Map *codes = map_new();
    if (*selected) {
      EACH(map_to_array(svCodesTb_read(selected)), Kv, nk_code) {
        map_add(codes, kv_key(nk_code), js_ws(kv_value(nk_code)));
      }_EACH
    }

    //<char>
    Map *rp = map_new();
    map_add(rp, "servers", js_wa(svs));
    map_add(rp, "dailyServer", js_ws(qsv_id(qsvs_current_diary())));
    map_add(rp, "historicServer", js_ws(qsv_id(qsvs_current_historic())));
    map_add(rp, "codes", js_wo(codes));
    pgs_print(rp);
  } else if (str_eq(rq, "setCodes")) {
    char *server = pgs_rs(mrq, "server");
    Map *codes = map_new();
    EACH(map_to_array(js_ro(pgs_r(mrq, "codes"))), Kv, nk_code) {
      map_add(codes, kv_key(nk_code), js_rs(kv_value(nk_code)));
    }_EACH
    svCodesTb_write(server, codes);
    pgs_print_empty();
  } else if (str_eq(rq, "testDiary")) {
    char *server = pgs_rs(mrq, "server");
    // <char>. Array of server codes.
    Arr *codes = arr_new();
    int withErrors = FALSE;
    int withWarnings = FALSE;
      //--
      int ffind (Qsv *sv) { return str_eq(qsv_id(sv), server); }
    Qsv *sv = opt_get(arr_find(qsvs_list(), (FPRED)ffind));
    if (sv) {
      //<char>
      Rs *html_rs = qsv_download_diary(sv);
      char *html = rs_get(html_rs);
      if (html) {
        // Tp<Arr<Kv<double>>, Arr<char>>
        Tp *tp = qsv_read_diary(sv, html);
        if (arr_size(tp_e2(tp))) {
          EACH(tp_e2(tp), char, err){ log_error(err); }_EACH
          withWarnings = TRUE;
          if (!arr_size(tp_e1(tp))) withErrors = TRUE;
        } else {
          // <double>
          Map *c_v = map_from_array(tp_e1(tp));
          codes = map_keys(c_v);
          // <char>
          Map *n_cs = svCodesTb_read(server);
          EACH(map_to_array(n_cs), Kv, n_c) {
            if (!map_has_key(c_v, kv_value(n_c))) {
              log_error(str_f("Value of '%s' not read", kv_key(n_c)));
              withErrors = TRUE;
            }
          }_EACH
        }
      } else {
        log_error(rs_error(html_rs));
        withErrors = TRUE;
      }
    } else {
      log_error(str_f("Server '%s' not found", server));
      withErrors = TRUE;
    }
    //<char>
    Map *rp = map_new();
    map_add(rp, "codes", js_wa(arr_map(codes, (FMAP)js_ws)));
    map_add(rp, "withErrors", js_wb(withErrors));
    map_add(rp, "withWarnings", js_wb(withWarnings));
    pgs_print(rp);
  } else if (str_eq(rq, "testCo")) {
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
  } else {
    EXC_GENERIC(str_f(
      "Value '%s' not found for 'rq' in '%s'", rq, js_wo(mrq)
    ));
  }
}
