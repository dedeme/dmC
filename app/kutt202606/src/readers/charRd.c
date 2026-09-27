// Copyright 11-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "readers/charRd.h"
#include "DEFS.h"

CharRd *charRd_new (char *tx) {
  CharRd *this = MALLOC(CharRd);
  this->tx = tx;
  this->i = 0;
  this->ln = 1;
  return this;
}

char charRd_peek (CharRd *this) {
  return this->tx[this->i];
}

char charRd_next (CharRd *this) {
  char r = this->tx[this->i];
  if (r == '\n') ++this->ln;
  if (r) ++this->i;
  return r;
}
