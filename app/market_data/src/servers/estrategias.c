// Copyright 13-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "servers/estrategias.h"
#include "servers/reader.h"
#include "DEFS.h"
#include "io.h"

static char *get_code(char *nick) {
  return
    str_eq(nick, "A3M") ? "Atresmedia"
    : str_eq(nick, "ACS") ? "ACS"
    : str_eq(nick, "ACX") ? "Acerinox"
    : str_eq(nick, "ADX") ? "Audax"
    : str_eq(nick, "AENA") ? "Aena"
    : str_eq(nick, "ALM") ? "Almirall"
    : str_eq(nick, "AMS") ? "Amadeus"
    : str_eq(nick, "ANA") ? "Acciona"
    : str_eq(nick, "APPS") ? "Applus"
    : str_eq(nick, "BBVA") ? "BBVA"
    : str_eq(nick, "BKIA") ? "Bankia"
    : str_eq(nick, "BKT") ? "Bankinter"
    : str_eq(nick, "CABK") ? "Caixabank"
    : str_eq(nick, "CIE") ? "CIE Automotive"
    : str_eq(nick, "CLNX") ? "Cellnex"
    : str_eq(nick, "COL") ? "Inmobiliaria Colonial"
    : str_eq(nick, "DIA") ? "DIA"
    : str_eq(nick, "ELE") ? "Endesa"
    : str_eq(nick, "ENC") ? "ENCE"
    : str_eq(nick, "ENG") ? "Enagas"
    : str_eq(nick, "FER") ? "Ferrovial"
    : str_eq(nick, "GEST") ? "Gestamp"
    : str_eq(nick, "GRF") ? "Grifols A"
    : str_eq(nick, "GSJ") ? "Grupo San Jose"
    : str_eq(nick, "IAG") ? "IAG (Iberia)"
    : str_eq(nick, "IBE") ? "Iberdrola"
    : str_eq(nick, "IDR") ? "Indra"
    : str_eq(nick, "ITX") ? "Inditex"
    : str_eq(nick, "MAP") ? "Mapfre"
    : str_eq(nick, "MAS") ? "Masmovil"
    : str_eq(nick, "MEL") ? "Melia Hotels"
    : str_eq(nick, "MRL") ? "MERLIN Properties"
    : str_eq(nick, "MTS") ? "ArcelorMittal"
    : str_eq(nick, "NTGY") ? "Naturgy (Gas Natural)"
    : str_eq(nick, "OHL") ? "OHL"
    : str_eq(nick, "PHM") ? "PHARMA MAR"
    : str_eq(nick, "PSG") ? "Prosegur"
    : str_eq(nick, "REE") ? "REE"
    : str_eq(nick, "REP") ? "Repsol"
    : str_eq(nick, "SAB") ? "Banco Sabadell"
    : str_eq(nick, "SAN") ? "Banco Santander"
    : str_eq(nick, "SCYR") ? "Sacyr"
    : str_eq(nick, "SGRE") ? "Siemens-Gamesa"
    : str_eq(nick, "SLR") ? "Solaria"
    : str_eq(nick, "TEF") ? "Telefonica"
    : str_eq(nick, "TL5") ? "Mediaset"
    : str_eq(nick, "TRE") ? "Tecnicas Reunidas"
    : str_eq(nick, "UNI") ? "UNICAJA"
    : str_eq(nick, "VIS") ? "Viscofan"
    : str_eq(nick, "ZOT") ? "Zardoya Otis"
    : ""
  ;
}

char *estrategias_name(void) {
  return "estrategias";
}

int estrategias_read_raw(Arr **codes_new, Darr **qs_new) {
  char *path =  "https://www.estrategiasdeinversion.com/cotizaciones/indices/"
                "mercado-continuo";
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
  varr_push(tstart, "<div class=\"md-table-economic-agenda\">");
  varr_push(tstart, "<tboody>");

  varr_push(tend, "</tboody>");

  // Row ---------------------------------------------------
  varr_push(rstart, "<tr>");

  varr_push(rend, "</tr>");

  // Cell 1 ------------------------------------------------
  varr_push(c1start, "<a ");
  varr_push(c1start, ">");

  varr_push(c1end, "<");

  // Cell 2 ------------------------------------------------
  varr_push(c2start, "<div ");
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

int estrategias_read(Varr **nicks_new, Darr **qs_new) {
  char *nks[] = NICKS;
  Varr *nicks = varr_new();
  Darr *qs = darr_new();
  *nicks_new = nicks;
  *qs_new = qs;

  Arr *codes;
  Darr *qcs;

  int e = estrategias_read_raw(&codes, &qcs);
  if (e) {
    return e;
  }

  char **pnks = nks;
  while (*pnks) {
    char *nk = *pnks++;
    char *code = get_code(nk);

    if (!*code) {
      io_loge("Nick '%s' not found in %s", nk, estrategias_name());
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
      io_loge("Quote of '%s' not fund in %s", nk, estrategias_name());
      return 1;
    }
  }

  return 0;
}
