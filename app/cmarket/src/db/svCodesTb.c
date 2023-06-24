// Copyright 05-06-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "db/svCodesTb.h"
#include "kut/DEFS.h"
#include "kut/js.h"
#include "data/qsvs.h"
#include "db/quotesDb.h"
#include "db/log.h"
#include "db.h"

static char *fpath = "svCodes.tb";

void svCodesTb_init (void) {
  if (!db_exists(fpath)) db_write(fpath, js_wo(map_new()));
}

// <char>
Map *svCodesTb_read (char *server_id) {
  // <s>
  Map *sv_codes = js_ro(db_read(fpath));

  // <Qsv>
  Arr *svs = qsvs_list();
  EACH(map_keys(sv_codes), char, id) {
      //--
      int any (Qsv *sv) { return str_eq(id, qsv_id(sv)); }
    if (!arr_any(svs, (FPRED)any))
      log_error(str_f(
        "Found codes of not existent server '%s'\n"
        "Codes can be deleted with 'svCodesTb_purge'",
        id
      ));
  }_EACH

  char *nk_code_js = opt_get(map_get(sv_codes, server_id));
  // <char>
  Arr *nicks = quotesDb_nicks();
  // <char>
  Map *nk_code = map_new();

  int changed = FALSE;
  if (nk_code_js) {
    EACH(map_to_array(js_ro(nk_code_js)), Kv, n_c) {
      map_add(nk_code, kv_key(n_c), js_rs(kv_value(n_c)));
    }_EACH

    EACH(nicks, char, nk) {
      if (!map_has_key(nk_code, nk)) {
        map_add(nk_code, nk, "");
        changed = TRUE;
      }
    }_EACH
    EACH(map_keys(nk_code), char, nk) {
        //--
        int any (char *nick) { return str_eq(nk, nick); }
      if (!arr_any(nicks, (FPRED)any)) {
        map_remove(nk_code, nk);
        changed = TRUE;
      }
    }_EACH
  } else {
    EACH(nicks, char, nk) {
      map_add(nk_code, nk, "");
    }_EACH
    changed = TRUE;
  }

  if (changed) svCodesTb_write (server_id, nk_code);

  return nk_code;
}

void svCodesTb_write (char *server_id, Map *nk_codes) {
  // <s>
  Map *nk_code_js = map_new();
  EACH(map_to_array(nk_codes), Kv, n_c) {
    map_add(nk_code_js, kv_key(n_c), js_ws(kv_value(n_c)));
  }_EACH

  // <s>
  Map *sv_codes = js_ro(db_read(fpath));
  map_put(sv_codes, server_id, js_wo(nk_code_js));

  db_write(fpath, js_wo(sv_codes));
}

void svCodesTb_purge (void) {
  // <s>
  Map *sv_codes = js_ro(db_read(fpath));
  // <Qsv>
  Arr *svs = qsvs_list();
  EACH(map_keys(sv_codes), char, sv_id) {
      //--
      int any (Qsv *sv) { return str_eq(sv_id, qsv_id(sv)); }
    if (!arr_any(svs, (FPRED)any)) map_remove(sv_codes, sv_id);
  }_EACH
  db_write(fpath, js_wo(sv_codes));
}

