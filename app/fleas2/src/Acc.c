// Copyright 25-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Acc.h"
#include "DEFS.h"
#include "io.h"

double fees(double money) {
  double broker = 9.75;
  if (money > 25000) {
    broker = money * 0.001;
  }

  double bolsa = 4.65 + money * 0.00012;
  if (money > 140000) {
    bolsa = 13.4;
  } else if (money > 70000) {
    bolsa = 9.2 + money * 0.00003;
  } else if (money > 35000) {
    bolsa = 6.4 + money * 0.00007;
  }

  return broker + bolsa;
}

typedef struct accEntry_AccEntry AccEntry;

struct accEntry_AccEntry {
  int co;
  int stocks;
};

AccEntry *accEntry_new(int co, int stocks) {
  AccEntry *this = malloc(sizeof(AccEntry));
  this->co = co;
  this->stocks = stocks;
  return this;
}

void accEntry_free(AccEntry *this) {
  free(this);
};

/* .
-Acc
  cash: double
  -pf: Arr *
*/

/*--*/

struct acc_Acc {
  double cash;
  Arr *pf;
};

Acc *_acc_new(double cash, Arr *pf) {
  Acc *this = malloc(sizeof(Acc));
  this->cash = cash;
  this->pf = pf;
  return this;
}

void acc_free(Acc *this) {
  arr_free(this->pf);
  free(this);
};

double acc_cash(Acc *this) {
  return this->cash;
}

/*--*/

Acc *acc_new(void) {
  return _acc_new(INITIAL_CAPITAL, arr_new((FPROC)accEntry_free));
}

void acc_buy(Acc *this, int company, int stocks, double price) {
  double value = stocks * price;
  this->cash -= (value + fees(value));
  arr_push(this->pf, accEntry_new(company, stocks));
}

int acc_sell(Acc *this, int company, double price) {
  EACH_IX(this->pf, AccEntry, e, ix)
    if (e->co == company) {
      double value = e->stocks * price;
      this->cash += (value - fees(value));
      arr_remove(this->pf, ix);
      return 1;
    }
  _EACH
  return 0;
}

double acc_liquidation(Acc *this) {
  double *closes = io_closes();
  int qnicks = io_qnicks();
  int icloses = (io_qdays() -1) * qnicks;
  double r = this->cash;
  EACH(this->pf, AccEntry, e)
    r += closes[icloses + e->co] * e->stocks;
  _EACH
  return r;
}
