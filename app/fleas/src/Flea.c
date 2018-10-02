// Copyright 27-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Flea.h"
#include "market/Buy.h"
#include "market/Sell.h"
#include "market/Pf.h"
#include "market/fees.h"
#include "Gen.h"
#include "families/UpDown.h"
#include "families/UpDownH.h"
#include "families/UpDownL.h"

/* .+.
struct: @Flea
  -id: char *: _string
  -family: char *: _string
  gens: Gen *: gen
  -
  buys: int = 0
  sells: int = 0
  assets: double = 0
  +ponderation: double = 0
*/

static void init(Family *family, Gen *gens, int companies_number) {
  char *f = (char *)family;

  if (str_eq(f, "ud")) {
    upDown_init(gens, companies_number);
    return;
  }
  if (str_eq(f, "ud-h")) {
    upDownH_init(gens, companies_number);
    return;
  }
  if (str_eq(f, "ud-l")) {
    upDownL_init(gens, companies_number);
    return;
  }
  THROW("")
    exc_illegal_argument(
      "family",
      "ud",
      f
    )
  _THROW
}

static void operate(
  Family *family,
  Abuy **buys,
  int **nicks_to_sell,
  int *nicks_to_sell_len,
  Quote **quotes
) {
  char *f = (char *)family;

  if (str_eq(f, "ud")) {
    upDown_operate(buys, nicks_to_sell, nicks_to_sell_len, quotes);
    return;
  }
  if (str_eq(f, "ud-h")) {
    upDownH_operate(buys, nicks_to_sell, nicks_to_sell_len, quotes);
    return;
  }
  if (str_eq(f, "ud-l")) {
    upDownL_operate(buys, nicks_to_sell, nicks_to_sell_len, quotes);
    return;
  }
  THROW("")
    exc_illegal_argument(
      "family",
      "ud",
      f
    )
  _THROW
}

/*.-.*/
#include "dmc/ct/Ajson.h"

struct flea_Flea {
  char *id;
  char *family;
  Gen *gens;

  int buys;
  int sells;
  double assets;
  double ponderation;
};

Flea *_flea_new(char *id, char *family, Gen *gens) {
  Flea *this = MALLOC(Flea);
  XNULL(id)
  this->id = id;
  XNULL(family)
  this->family = family;
  XNULL(gens)
  this->gens = gens;
  this->buys = 0;
  this->sells = 0;
  this->assets = 0;
  this->ponderation = 0;
  return this;
}

Gen *flea_gens(Flea *this) {
  XNULL(this)
  return this->gens;
}

int flea_buys(Flea *this) {
  XNULL(this)
  return this->buys;
}

int flea_sells(Flea *this) {
  XNULL(this)
  return this->sells;
}

double flea_assets(Flea *this) {
  XNULL(this)
  return this->assets;
}

double flea_ponderation(Flea *this) {
  XNULL(this)
  return this->ponderation;
}

void flea_set_ponderation(Flea *this, double value) {
  XNULL(this)
  this->ponderation = value;
}

Json *flea_to_json(Flea *this) {
  XNULL(this)
  Ajson *serial = ajson_new();
  jarr_astring(serial, this->id);
  jarr_astring(serial, this->family);
  ajson_add(serial, gen_to_json(this->gens));
  return json_warray(serial);
}

Flea *flea_from_json(Json *js) {
  XNULL(js)
  Ajson *serial = json_rarray(js);
  Flea *this = MALLOC(Flea);
  size_t i = 0;
  this->id = jarr_gstring(serial, i++);
  this->family = jarr_gstring(serial, i++);
  this->gens = gen_from_json(ajson_get(serial, i++));
  return this;
}
/*.-.*/

///
Flea *flea_prototype(Family *family) {
  char *f = (char *)family;

  if (str_eq(f, "ud")) {
    return _flea_new("0", f, upDown_prototype());
  }
  if (str_eq(f, "ud-h")) {
    return _flea_new("0", f, upDownH_prototype());
  }
  if (str_eq(f, "ud-l")) {
    return _flea_new("0", f, upDownL_prototype());
  }
  THROW("")
    exc_illegal_argument(
      "family",
      "ud",
      f
    )
  _THROW
  return NULL;
}

///
Flea *flea_mutate(Flea *this, int cycle, int id) {
  Gen *gens = gen_mutate(this->gens);
  char *name = str_printf("%d-%d", cycle, id);
  return _flea_new(name, this->family, gens);
}

///
void flea_operate(
  Flea *this,
  double money,
  Quote ***quotes,
  int days_number,
  int companies_number
) {
  Family *family = (Family *)this->family;

  init(family, this->gens, companies_number);

  // Acumulators
  Pf *pf = pf_new();
  int nbuys = 0;
  int nsells = 0;
  Quote **last_quote = NULL;

  // Cyclic
  Abuy *buys = abuy_new();
  int *sell_nks = NULL;
  int sell_nks_len = 0;
  Quote ***dqs = quotes;
  int nday = 0;
  for(;;) {                             // Start cycle -------------------------
    Quote **dq = *dqs++;

    // Mk buys

    Abuy *bnew = abuy_new();
    double tmp_money = money;
    EACH(buys, Buy, b) {
      tmp_money -= buy_do(b, buy_price(b));
      if (tmp_money < 1000) {
        break;
      }
      abuy_add(bnew, b);
    }_EACH

    EACH(bnew, Buy, b) {
      size_t nick = buy_nick(b);
      double price = quote_open(dq[nick]);
      if (price < 0.01) {
        continue;
      }
      size_t stocks = buy_stocks(b);
      pf_add(pf, nick, stocks, price);
      money -= buy_do(b, price);
      ++nbuys;
    }_EACH

    // Mk sells

    Achar *to_do = achar_new();
    RANGE0(i, sell_nks_len) {
      int nick = sell_nks[i];
      double price = quote_open(dq[nick]);
      if (price < 0.01) {
        achar_add(to_do, str_printf("%d", nick));
        continue;
      }
      size_t stocks = pf_stocks(pf, nick);
      if (stocks) {
        pf_remove(pf, nick, stocks);
        money += sell_income(stocks, price);
        ++nsells;
      }
    }_RANGE

    operate(family, &buys, &sell_nks, &sell_nks_len, dq);

    // Add sell orders which could not be done
    EACH(to_do, char, nick) {
      sell_nks[sell_nks_len++] = atoi(nick);
    }_EACH

    ++nday;
    if (nday == days_number) {
      last_quote = dq;
      break;
    }
  }                                     // End cycle ---------------------------

  // After cycle

  size_t *nicks = pf_nicks(pf);
  REPEAT(pf_size(pf)) {
    size_t nick = *nicks++;
    double close = quote_close(last_quote[nick]);
    if (close >= 0) {
      money += close * pf_stocks(pf, nick);
    } else if (close > -1.5) {
      PfEntry *e = pf_get(pf, nick);
      if (e) {
        money += pfEntry_price(e) * pf_stocks(pf, nick);
      }
    }
  }_REPEAT

  this->buys = nbuys;
  this->sells = nsells;
  this->assets = money;
}

static char *fms[3] = {"ud-l", "ud", "ud-h"};
void flea_families_list(Family ***families, int *families_number) {
  *families = (Family **)fms;
  *families_number = 3;
}

char *flea_family_to_str(Family *family) {
  return (char *)family;
}
