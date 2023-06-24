// Copyright 06-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/svs/infob.h"
#include "kut/DEFS.h"
#include "kut/str.h"
#include "kut/dec.h"
#include "kut/rs.h"
#include "kut/sys.h"
#include "kut/time.h"
#include "mkt/qs.h"

static char *read_vol (char *tx) {
  if (!*tx) return "0";
  double f = str_ends(tx, "K")
    ? dec_from_iso(str_left(tx, -1)) * 1000
    : str_ends(tx, "M")
      ? dec_from_iso(str_left(tx, -1)) * 1000000
      : dec_from_iso(tx)
  ;
  return dec_itos((int)f);
}

// DIARY -----------------------------------------------------------------------

// <Arr<Kv<double>>, Arr<char>>
static Tp *read_diary (char *html) {
  EXC_ILLEGAL_STATE("Not implemented");
  return NULL; // Unreachable
}

// HISTORIC --------------------------------------------------------------------

// <Opt<Qs>, Arr<char>>
static Tp *read_historic (char *html) {
  // <char> (quotes type file 'NICK.tb')
  Arr *qs = arr_new();
  // <char>
  Arr *errs = arr_new();
  int ix = 0;
  int ix2 = 0;

  // <Qs>. qs is Arr<char> (quotes type file 'NICK.tb')
  Opt *mk_qs () {
    // <Qs>
    Rs *qs_rs = qs_from_str(arr_size(qs), arr_cjoin(qs, '\n'));
    Qs *rqs = rs_get(qs_rs);
    if (rqs) return opt_some(rqs);
    arr_push(errs, rs_error(qs_rs));
    return opt_none();
  }

  // <Opt<Qs>, Arr<char>>
  Tp *ret_error (char *msg) {
    arr_push(errs, msg);
    // <Qs>
    Opt *oqs = mk_qs();
    return tp_new(oqs, errs);
  }

  ix = str_index(html, "\"button\"><span>% var.</span>");
  if (ix == -1) return ret_error("Beginning of table not found");
  ix += 28;

  int end_table = str_index_from(html, "</tbody>", ix);
  if (end_table == -1) return ret_error("End of table not found");

  for (;;) {
    ix = str_index_from(html, "<tr ", ix);
    if (ix == -1 || ix > end_table) break;
    ix += 4;

    int end_row = str_index_from(html, "</tr>", ix);
    if (end_row == -1) return ret_error("End of row not found");

    // DATE
    ix = str_index_from(html, "Time=\"", ix);
    if (ix == -1) {
      arr_push(errs, "Beginning of field 'date' not found");
      ix = end_row;
      continue;
    }
    ix += 6;
    ix2 = str_index_from(html, "\"", ix);
    if (ix2 == -1) {
      arr_push(errs, "End of field 'date' not found");
      ix = end_row;
      continue;
    }
    char *date = "";
    TRY {
      date = time_to_str(time_from_iso(
        str_creplace(str_trim(str_sub(html, ix, ix2)), '.', '/')
      ));
    } CATCH(e) {
      arr_push(errs, str_f("Bad date: %s", exc_msg(e)));
    }_TRY
    if (!*date) continue;

    // CLOSE
    ix = str_index_from(html, "ltr\">", ix);
    if (ix == -1) {
      arr_push(errs, "Beginning of field 'close' not found");
      ix = end_row;
      continue;
    }
    ix += 5;
    ix2 = str_index_from(html, "<", ix);
    if (ix2 == -1) {
      arr_push(errs, "End of field 'close' not found");
      ix = end_row;
      continue;
    }
    char *close = "";
    TRY {
      close = dec_ftos(dec_from_iso(str_trim(str_sub(html, ix, ix2))), 4);
    } CATCH(e) {
      arr_push(errs, str_f("Bad close: %s", exc_msg(e)));
    }_TRY
    if (!*close) continue;

    // OPEN
    ix = str_index_from(html, "\">", ix);
    if (ix == -1) {
      arr_push(errs, "Beginning of field 'open' not found");
      ix = end_row;
      continue;
    }
    ix += 2;
    ix2 = str_index_from(html, "<", ix);
    if (ix2 == -1) {
      arr_push(errs, "End of field 'open' not found");
      ix = end_row;
      continue;
    }
    char *open = "";
    TRY {
      open = dec_ftos(dec_from_iso(str_trim(str_sub(html, ix, ix2))), 4);
    } CATCH(e) {
      arr_push(errs, str_f("Bad open: %s", exc_msg(e)));
    }_TRY
    if (!*open) continue;

    // MAX
    ix = str_index_from(html, "\">", ix);
    if (ix == -1) {
      arr_push(errs, "Beginning of field 'max' not found");
      ix = end_row;
      continue;
    }
    ix += 2;
    ix2 = str_index_from(html, "<", ix);
    if (ix2 == -1) {
      arr_push(errs, "End of field 'max' not found");
      ix = end_row;
      continue;
    }
    char *max = "";
    TRY {
      max = dec_ftos(dec_from_iso(str_trim(str_sub(html, ix, ix2))), 4);
    } CATCH(e) {
      arr_push(errs, str_f("Bad max: %s", exc_msg(e)));
    }_TRY
    if (!*max) continue;

    // MIN
    ix = str_index_from(html, "\">", ix);
    if (ix == -1) {
      arr_push(errs, "Beginning of field 'min' not found");
      ix = end_row;
      continue;
    }
    ix += 2;
    ix2 = str_index_from(html, "<", ix);
    if (ix2 == -1) {
      arr_push(errs, "End of field 'min' not found");
      ix = end_row;
      continue;
    }
    char *min = "";
    TRY {
      min = dec_ftos(dec_from_iso(str_trim(str_sub(html, ix, ix2))), 4);
    } CATCH(e) {
      arr_push(errs, str_f("Bad min: %s", exc_msg(e)));
    }_TRY
    if (!*min) continue;

    // VOL
    ix = str_index_from(html, "\">", ix);
    if (ix == -1) {
      arr_push(errs, "Beginning of field 'vol' not found");
      ix = end_row;
      continue;
    }
    ix += 2;
    ix2 = str_index_from(html, "<", ix);
    if (ix2 == -1) {
      arr_push(errs, "End of field 'vol' not found");
      ix = end_row;
      continue;
    }
    char *vol = "";
    TRY {
      vol = read_vol(str_trim(str_sub(html, ix, ix2)));
    } CATCH(e) {
      arr_push(errs, str_f("Bad vol: %s", exc_msg(e)));
    }_TRY
    if (!*vol) continue;

    // END
    arr_push(qs, str_f(
      "%s:%s:%s:%s:%s:%s:false",
      date, close, open, max, min, vol
    ));
    ix = end_row;
  }

  // <Qs>
  Opt *oqs = mk_qs();
  return tp_new(oqs, errs);
}

// OTHER -----------------------------------------------------------------------

static char *id (void) {
  return "INVES";
}

static char *name (void) {
  return "Investing";
}

static int with_diary (void) {
  return FALSE;
}

// <char>
static Rs *download_diary (void) {
  EXC_ILLEGAL_STATE("Not implemented");
  return NULL; // Unreachable
}

static int with_historic (void) {
  return TRUE;
}

// <char>
static Rs *download_historic (char *code) {
  int ix = str_cindex(code, '?');
  char *tail = ix == -1
    ? ""
    : str_right(code, ix)
  ;
  code = ix == -1
    ? code
    : str_left(code, ix)
  ;
  char *url = str_f(
    "https://es.investing.com/equities/%s-historical-data%s",
    code, tail
  );
  return sys_cmd(str_f(
    "timeout -s KILL 15 wget -q --no-cache -O - %s", url
  ));
}

Qsv *inves_new (void) {
  return qsv_new(
    id, name, with_diary, download_diary, read_diary,
    with_historic, download_historic, read_historic
  );
}
