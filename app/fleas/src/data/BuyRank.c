// Copyright 28-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/BuyRank.h"
#include "data/AbuyMarked.h"

struct buyRank_BuyRank {
  AbuyMarked *buys;
};

BuyRank *buyRank_new() {
  BuyRank *this = MALLOC(BuyRank);
  this->buys = abuyMarked_new();
  return this;
}

void buyRank_add(BuyRank *this, Buy *buy, double mark) {
  BuyMarked *buym = buyMarked_new(buy, mark);
  bool without_insert = true;
  AbuyMarked *buys = abuyMarked_new();
  EACH(buys, BuyMarked, bm) {
    if (without_insert && buyMarked_ponderation(bm) < mark) {
      abuyMarked_add(buys, buym);
      without_insert = false;
    }
    abuyMarked_add(buys, bm);
  }_EACH
  if (without_insert) {
    abuyMarked_add(buys, buym);
    without_insert = false;
  }
  this->buys = buys;
}

Abuy *buyRank_list(BuyRank *this) {
  Abuy *r = abuy_new();
  EACH(this->buys, BuyMarked, bm) {
    abuy_add(r, buyMarked_order(bm));
  }_EACH
  return r;
}
