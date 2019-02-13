// Copyright 13-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "servers/eleconomista.h"
#include "servers/reader.h"
#include "DEFS.h"
#include "io.h"
#include "servers.h"

char *eleconomista_name(void) {
  return "eleconomista";
}

int eleconomista_read_raw(Arr **codes_new, Darr **qs_new, char *path) {
  // Varr[char]
  Varr *tstart = varr_new();
  // Varr[char]
  Varr *tend = varr_new();
  // Varr[char]
  Varr *rstart = varr_new();
  // Varr[char]
  Varr *rend = varr_new();
  // Varr[char]
  Varr *c1start = varr_new();
  // Varr[char]
  Varr *c1end = varr_new();
  // Varr[char]
  Varr *c2start = varr_new();
  // Varr[char]
  Varr *c2end = varr_new();

  // Table -------------------------------------------------
  varr_push(tstart, "Principales acciones");
  varr_push(tstart, "<tbody>");

  varr_push(tend, "var google_conversion_id = 1063995664");

  // Row ---------------------------------------------------
  varr_push(rstart, "<tr itemscope=");

  varr_push(rend, "</tr>");

  // Cell 1 ------------------------------------------------
  varr_push(c1start, "<a href=");
  varr_push(c1start, ">");

  varr_push(c1end, "<");

  // Cell 2 ------------------------------------------------
  varr_push(c2start, "<span ");
  varr_push(c2start, ">");

  varr_push(c2end, "<");

  int r = reader_run(
    codes_new, qs_new,
    path, 0,
    tstart, tend, rstart, rend, c1start, c1end, c2start, c2end
  );

  varr_free(tstart);
  varr_free(tend);
  varr_free(rstart);
  varr_free(rend);
  varr_free(c1start);
  varr_free(c1end);
  varr_free(c2start);
  varr_free(c2end);

  return r;
}

// Not modify -----------------------------------------------------------------
// ▼▼▼▼▼▼▼▼▼▼ -----------------------------------------------------------------
int eleconomista_read(Varr **nicks_new, Darr **qs_new) {
  char *svname = eleconomista_name();                     //-------------------
  Varr *nicks = varr_new();
  Darr *qs = darr_new();
  *nicks_new = nicks;
  *qs_new = qs;

  char *url_new;
  // Map[char]
  Map *codes_new;
  servers_data(&url_new, &codes_new, svname);


  Arr *codes;
  Darr *qcs;
  int e = eleconomista_read_raw(&codes, &qcs, url_new);   //-------------------
  free(url_new);
  if (e) {
    map_free(codes_new);
    return e;
  }

  EACH(codes_new, Kv, kv)
    char *nk = map_key(kv);
    char *code = map_value(kv);

    if (!*code) {
      io_loge("Nick '%s' no encontrado en %s", nk, svname);
      return 1;
    }

    double q = -1;
    EACH_IX(codes, char, c, ix)
      if (str_eq(c, code)) {
        q = darr_get(qcs, ix);
        varr_push(nicks, nk);
        darr_push(qs, q);
        break;
      }
    _EACH
    if (q == -1) {
      io_loge("Quote of '%s' not fund in %s", nk, svname);
      return 1;
    }
  _EACH

  return 0;
}
