// Copyright 13-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "servers/invertia.h"
#include "servers/reader.h"
#include "DEFS.h"
#include "io.h"

static char *get_code(char *nick) {
  return
    str_eq(nick, "A3M") ? "A3M"
    : str_eq(nick, "ACS") ? "ACS"
    : str_eq(nick, "ACX") ? "ACX"
    : str_eq(nick, "AENA") ? "AENA"
    : str_eq(nick, "ALM") ? "ALM"
    : str_eq(nick, "AMS") ? "AMA"
    : str_eq(nick, "ANA") ? "ANA"
    : str_eq(nick, "BBVA") ? "BBVA"
    : str_eq(nick, "BKIA") ? "BKIA"
    : str_eq(nick, "BKT") ? "BKT"
    : str_eq(nick, "CABK") ? "CABK"
    : str_eq(nick, "CAF") ? "CAF"
    : str_eq(nick, "CIE") ? "CIE"
    : str_eq(nick, "CLNX") ? "CLNX"
    : str_eq(nick, "COL") ? "COL"
    : str_eq(nick, "DIA") ? "DIA"
    : str_eq(nick, "ECR") ? "ECR"
    : str_eq(nick, "EKT") ? "EKTL"
    : str_eq(nick, "ELE") ? "ELE"
    : str_eq(nick, "ENC") ? "ENC"
    : str_eq(nick, "ENG") ? "ENG"
    : str_eq(nick, "FAE") ? "FAE"
    : str_eq(nick, "FCC") ? "FCC"
    : str_eq(nick, "FER") ? "FER"
    : str_eq(nick, "GEST") ? "GEST"
    : str_eq(nick, "GRF") ? "GRF"
    : str_eq(nick, "IAG") ? "IAG"
    : str_eq(nick, "IBE") ? "IBE"
    : str_eq(nick, "IDR") ? "IDR"
    : str_eq(nick, "ITX") ? "ITX"
    : str_eq(nick, "MAP") ? "MAP"
    : str_eq(nick, "MEL") ? "MEL"
    : str_eq(nick, "MRL") ? "MRL"
    : str_eq(nick, "MTS") ? "MTS"
    : str_eq(nick, "NHH") ? "NHH"
    : str_eq(nick, "NTGY") ? "NTGY"
    : str_eq(nick, "OHL") ? "OHL"
    : str_eq(nick, "PHM") ? "PHM"
    : str_eq(nick, "PSG") ? "PSG"
    : str_eq(nick, "REE") ? "REE"
    : str_eq(nick, "REP") ? "REP"
    : str_eq(nick, "SAB") ? "SAB"
    : str_eq(nick, "SAN") ? "SAN"
    : str_eq(nick, "SCYR") ? "SCYR"
    : str_eq(nick, "SGREN") ? "SGREN"
    : str_eq(nick, "SLR") ? "SLR"
    : str_eq(nick, "TEF") ? "TEF"
    : str_eq(nick, "TL5") ? "TL5"
    : str_eq(nick, "TLGO") ? "TLGO"
    : str_eq(nick, "TRE") ? "TRE"
    : str_eq(nick, "TUB") ? "TUB"
    : str_eq(nick, "VIS") ? "VIS"
    : str_eq(nick, "ZOT") ? "ZOT"
    : ""
  ;
}

char *invertia_name(void) {
  return "invertia";
}

int invertia_read_raw(Arr **codes_new, Darr **qs_new) {
  char *path =  "https://www.invertia.com/es/mercados/bolsa/indices/acciones/"
                "-/indice/mdo-continuo/IB011CONTINU";
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
  varr_push(tstart, "<tbody ");
  varr_push(tstart, "<tbody ");

  varr_push(tend, "</tbody>");

  // Row ---------------------------------------------------
  varr_push(rstart, "<tr class=\" \" >");

  varr_push(rend, "</tr>");

  // Cell 1 ------------------------------------------------
  varr_push(c1start, "<a ");
  varr_push(c1start, ">");

  varr_push(c1end, "<");

  // Cell 2 ------------------------------------------------
  varr_push(c2start, "<td ");
  varr_push(c2start, ">");

  varr_push(c2end, "<");

  int r = reader_run(
    codes_new, qs_new,
    path, 1,
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

int invertia_read(Varr **nicks_new, Darr **qs_new) {
  char *nks[] = NICKS;
  Varr *nicks = varr_new();
  Darr *qs = darr_new();
  *nicks_new = nicks;
  *qs_new = qs;

  Arr *codes;
  Darr *qcs;

  int e = invertia_read_raw(&codes, &qcs);
  if (e) {
    return e;
  }

  char **pnks = nks;
  while (*pnks) {
    char *nk = *pnks++;
    char *code = get_code(nk);

    if (!*code) {
      io_loge("Nick '%s' not found in %s", nk, invertia_name());
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
      io_loge("Quote of '%s' not fund in %s", nk, invertia_name());
      return 1;
    }
  }

  return 0;
}
