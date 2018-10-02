// Copyright 28-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/BuyMarked.h"

/* .+.
-struct: BuyMarked
  order: Buy *
  ponderation: double
*/
/*.-.*/
#include "dmc/ct/Ajson.h"

struct buyMarked_BuyMarked {
  Buy *order;
  double ponderation;
};

BuyMarked *buyMarked_new(Buy *order, double ponderation) {
  BuyMarked *this = MALLOC(BuyMarked);
  XNULL(order)
  this->order = order;
  this->ponderation = ponderation;
  return this;
}

Buy *buyMarked_order(BuyMarked *this) {
  XNULL(this)
  return this->order;
}

double buyMarked_ponderation(BuyMarked *this) {
  XNULL(this)
  return this->ponderation;
}
/*.-.*/
