// Copyright 13-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "servers/libremercado.h"
#include "servers/reader.h"
#include "DEFS.h"
#include "io.h"

static char *get_code(char *nick) {
  return
      str_eq(nick, "A3M") ? "ATRESMEDIA"
    : str_eq(nick, "ACS") ? "ACS"
    : str_eq(nick, "ACX") ? "ACERINOX"
    : str_eq(nick, "ADX") ? "AUDAX"
    : str_eq(nick, "AENA") ? "AENA"
    : str_eq(nick, "ALM") ? "ALMIRALL"
    : str_eq(nick, "AMS") ? "AMADEUS"
    : str_eq(nick, "ANA") ? "ACCIONA"
    : str_eq(nick, "APPS") ? "APPLUS SERVI"
    : str_eq(nick, "BBVA") ? "BBVA"
    : str_eq(nick, "BKIA") ? "BANKIA"
    : str_eq(nick, "BKT") ? "BANKINTER"
    : str_eq(nick, "CABK") ? "CAIXABANK"
    : str_eq(nick, "CIE") ? "CIE AUTOMOT."
    : str_eq(nick, "CLNX") ? "CELLNEX"
    : str_eq(nick, "COL") ? "INM.COLONIAL"
    : str_eq(nick, "DIA") ? "DIA"
    : str_eq(nick, "ELE") ? "ENDESA"
    : str_eq(nick, "ENC") ? "ENCE"
    : str_eq(nick, "ENG") ? "ENAGAS"
    : str_eq(nick, "FER") ? "FERROVIAL"
    : str_eq(nick, "GEST") ? "GESTAMP"
    : str_eq(nick, "GRF") ? "GRIFOLS"
    : str_eq(nick, "GSJ") ? "SAN JOSE"
    : str_eq(nick, "IAG") ? "IAG"
    : str_eq(nick, "IBE") ? "IBERDROLA"
    : str_eq(nick, "IDR") ? "INDRA A"
    : str_eq(nick, "ITX") ? "INDITEX"
    : str_eq(nick, "MAP") ? "MAPFRE"
    : str_eq(nick, "MAS") ? "MASMOVIL"
    : str_eq(nick, "MEL") ? "MELIA HOTELS"
    : str_eq(nick, "MRL") ? "MERLIN PROP."
    : str_eq(nick, "MTS") ? "ARCELORMITT."
    : str_eq(nick, "NTGY") ? "NATURGY"
    : str_eq(nick, "OHL") ? "OHL"
    : str_eq(nick, "PHM") ? "PHARMA MAR"
    : str_eq(nick, "PSG") ? "PROSEGUR"
    : str_eq(nick, "REE") ? "R.E.C."
    : str_eq(nick, "REP") ? "REPSOL"
    : str_eq(nick, "SAB") ? "B.SABADELL"
    : str_eq(nick, "SAN") ? "SANTANDER"
    : str_eq(nick, "SCYR") ? "SACYR"
    : str_eq(nick, "SGRE") ? "SIEMENS GAM"
    : str_eq(nick, "SLR") ? "SOLARIA"
    : str_eq(nick, "TEF") ? "TELEFONICA"
    : str_eq(nick, "TL5") ? "MEDIASET"
    : str_eq(nick, "TRE") ? "TECNICAS REU"
    : str_eq(nick, "UNI") ? "UNICAJA"
    : str_eq(nick, "VIS") ? "VISCOFAN"
    : str_eq(nick, "ZOT") ? "ZARDOYA OTIS"
    : ""
  ;
}

char *libremercado_name(void) {
  return "libremercado";
}

int libremercado_read_raw(Arr **codes_new, Darr **qs_new) {
  char *path =  "https://www.libremercado.com/mercados/mercado-continuo/";
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
  varr_push(tstart, "<tbody>");

  varr_push(tend, "</tbody>");

  // Row ---------------------------------------------------
  varr_push(rstart, "<tr>");

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
int libremercado_read(Varr **nicks_new, Darr **qs_new) {
  char *nks[] = NICKS;
  Varr *nicks = varr_new();
  Darr *qs = darr_new();
  *nicks_new = nicks;
  *qs_new = qs;

  Arr *codes;
  Darr *qcs;

  int e = libremercado_read_raw(&codes, &qcs);
  if (e) {
    return e;
  }

  char **pnks = nks;
  while (*pnks) {
    char *nk = *pnks++;
    char *code = get_code(nk);

    if (!*code) {
      io_loge("Nick '%s' not found in %s", nk, libremercado_name());
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
      io_loge("Quote of '%s' not fund in %s", nk, libremercado_name());
      return 1;
    }
  }

  return 0;
}