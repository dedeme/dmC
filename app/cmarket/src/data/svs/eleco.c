// Copyright 09-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/svs/eleco.h"
#include "kut/DEFS.h"
#include "kut/str.h"
#include "kut/dec.h"
#include "kut/rs.h"
#include "kut/sys.h"
#include "kut/time.h"
#include "mkt/qs.h"

// DIARY -----------------------------------------------------------------------

// <Arr<Kv<double>>, Arr<char>>
static Tp *read_diary (char *html) {
  // <Kv<double>>
  Arr *values = arr_new();
  // <char>
  Arr *errs = arr_new();
  int ix = 0;
  int ix2 = 0;

  // <Arr<Kv<double>>, Arr<char>>
  Tp *ret_error (char *msg) {
    arr_push(errs, msg);
    return tp_new(values, errs);
  }

  ix = str_index(html, "<tbody>");
  if (ix == -1) return ret_error("Beginning of table not found");
  ix += 7;

  // Look for the second <tbody> is ok.
  ix = str_index_from(html, "<tbody>", ix);
  if (ix == -1) return ret_error("Beginning of table not found");
  ix += 7;

  int end_table = str_index_from(html, "<footer", ix);
  if (end_table == -1) return ret_error("End of table not found");

  for (;;) {
    ix = str_index_from(html, "<tr i", ix);
    if (ix == -1 || ix > end_table) break;
    ix += 5;

    int end_row = str_index_from(html, "</tr>", ix);
    if (end_row == -1) return ret_error("End of row not found");

    // CODE
    ix = str_index_from(html, "/empresa/", ix);
    if (ix == -1) {
      arr_push(errs, "Beginning of field 'code' not found");
      ix = end_row;
      continue;
    }
    ix += 9;
    ix2 = str_index_from(html, "\"", ix);
    if (ix2 == -1) {
      arr_push(errs, "End of field 'code' not found");
      ix = end_row;
      continue;
    }
    char *code = str_sub(html, ix, ix2);

    // VALUE
    ix = str_index_from(html, "_\">", ix2 + 1);
    if (ix == -1) {
      arr_push(errs, "Beginning of field 'value' not found");
      ix = end_row;
      continue;
    }
    ix += 3;
    ix2 = str_index_from(html, "<", ix);
    if (ix2 == -1) {
      arr_push(errs, "End of field 'code' not found");
      ix = end_row;
      continue;
    }
    TRY {
      double value = dec_from_iso(str_sub(html, ix, ix2));
      double *vvalue = ATOMIC(sizeof(double));
      *vvalue = value;
      arr_push(values, kv_new(code, vvalue));
    } CATCH(e) {
      arr_push(errs, str_f("Bad value: %s", exc_msg(e)));
    }_TRY

    // END
    ix = end_row;
  }

  return tp_new(values, errs);
}

// HISTORIC --------------------------------------------------------------------

// <Opt<Qs>, Arr<char>>
static Tp *read_historic (char *html) {
  EXC_ILLEGAL_STATE("Not implemented");
  return NULL; // Unreachable
}

// OTHER -----------------------------------------------------------------------

static char *id (void) {
  return "ELECO";
}

static char *name (void) {
  return "El Economista";
}

static int with_diary (void) {
  return TRUE;
}

// <char>
static Rs *download_diary (void) {
  char *url = "https://www.eleconomista.es/mercados/mercado-continuo";
  return sys_cmd(str_f(
    "timeout -s KILL 10 wget -q --no-cache -O - %s", url
  ));
}

static int with_historic (void) {
  return FALSE;
}

// <char>
static Rs *download_historic (char *code) {
  EXC_ILLEGAL_STATE("Not implemented");
  return NULL; // Unreachable
}

Qsv *eleco_new (void) {
  return qsv_new(
    id, name, with_diary, download_diary, read_diary,
    with_historic, download_historic, read_historic
  );
}
