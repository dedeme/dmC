// Copyright 09-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/svs/yahoo.h"
#include "kut/DEFS.h"
#include "kut/str.h"
#include "kut/dec.h"
#include "kut/rs.h"
#include "kut/sys.h"
#include "kut/time.h"
#include "mkt/qs.h"

static char *get_date (char *date) {
  char *get_month (char *m) {
    m = str_f("%s  ", m);
    switch (m[0]) {
      case 'd': return "12";
      case 'n': return "11";
      case 'o': return "10";
      case 's': return "09";
      case 'f': return "02";
      case 'a': return m[1] == 'u' ? "08" : "04";
      case 'm': return m[3] == 'y' ? "05" : "03";
      default: return m[2] == 'a' ? "01" : m[3] == 'l' ? "07" : "06";
    }
  }

  // <char>
  Arr *ps = str_csplit_trim(date, ' ');

  return str_f(
    "%s%s%s",
    arr_get(ps, 2),
    get_month(arr_get(ps, 1)),
    arr_get(ps, 0)
  );
}

// <Arr<Kv<double>>, Arr<char>>
static Tp *read_diary (char *html) {
  EXC_ILLEGAL_STATE("Not implemented");
  return NULL; // Unreachable
}

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

  ix = str_index(html, "<tbody>");
  if (ix == -1) return ret_error("Beginning of table not found");
  ix += 7;

  int end_table = str_index_from(html, "</tbody>", ix);
  if (end_table == -1) return ret_error("End of table not found");

  for (;;) {
    ix = str_index_from(html, "<tr", ix);
    if (ix == -1 || ix > end_table) break;
    ix += 3;

    int end_row = str_index_from(html, "</tr>", ix);
    if (end_row == -1) return ret_error("End of row not found");

    // DATE
    ix = str_index_from(html, "<span>", ix);
    if (ix == -1) {
      arr_push(errs, "Beginning of field 'date' not found");
      ix = end_row;
      continue;
    }
    ix += 6;
    ix2 = str_index_from(html, "<", ix);
    if (ix2 == -1) {
      arr_push(errs, "End of field 'date' not found");
      ix = end_row;
      continue;
    }
    char *date = "";
    TRY {
      date = time_to_str(time_from_str(
        get_date(str_trim(str_sub(html, ix, ix2)))
      ));
    } CATCH(e) {
      arr_push(errs, str_f("Bad date: %s", exc_msg(e)));
    }_TRY
    if (!*date) {
      ix = end_row;
      continue;
    }

    // OPEN
    ix = str_index_from(html, "<span>", ix);
    if (ix == -1) {
      arr_push(errs, "Beginning of field 'open' not found");
      ix = end_row;
      continue;
    }
    ix += 6;
    if (html[ix] == 'D') {
      ix = end_row;
      continue;
    }

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
    ix = str_index_from(html, "<span>", ix);
    if (ix == -1) {
      arr_push(errs, "Beginning of field 'max' not found");
      ix = end_row;
      continue;
    }
    ix += 6;
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
    ix = str_index_from(html, "<span>", ix);
    if (ix == -1) {
      arr_push(errs, "Beginning of field 'min' not found");
      ix = end_row;
      continue;
    }
    ix += 6;
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

    // CLOSE
    ix = str_index_from(html, "<span>", ix);
    if (ix == -1) {
      arr_push(errs, "Beginning of field 'close' not found");
      ix = end_row;
      continue;
    }
    ix += 6;
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

    // VOL

    ix = str_index_from(html, "<span>", ix);
    if (ix == -1) {
      arr_push(errs, "Beginning of field 'vol' not found");
      ix = end_row;
      continue;
    }
    ix += 6;

    ix = str_index_from(html, "<span>", ix);
    if (ix == -1) {
      arr_push(errs, "Beginning of field 'vol' not found");
      ix = end_row;
      continue;
    }
    ix += 6;

    char *vol = "";
    // Note: When volume is 0, yahoo writes '-' instead '<span>0</span>'.
    if (ix < end_row) {
      ix2 = str_index_from(html, "<", ix);
      if (ix2 == -1) {
        arr_push(errs, "End of field 'vol' not found");
        ix = end_row;
        continue;
      }
      TRY {
        vol = dec_itos((int)dec_from_iso(str_trim(str_sub(html, ix, ix2))));
      } CATCH(e) {
        arr_push(errs, str_f("Bad vol: %s", exc_msg(e)));
      }_TRY
      if (!*vol) continue;
    } else {
      vol = "0";
    }

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

static char *id (void) {
  return "YAHOO";
}

static char *name (void) {
  return "Yahoo";
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
  char *url = str_f(
    "https://es.finance.yahoo.com/quote/%s/history?p=%s", code, code
  );
  char *c = str_f(
    "timeout -s KILL 15 wget "
    "--user-agent Mozilla "
    "--load-cookies=/home/deme/.mozilla/firefox/bfrqeymk.default/cookies.sqlite "
    "-q --no-cache -O - %s", url
  );
  return sys_cmd(c);
}

Qsv *yahoo_new (void) {
  return qsv_new(
    id, name, with_diary, download_diary, read_diary,
    with_historic, download_historic, read_historic
  );
}
