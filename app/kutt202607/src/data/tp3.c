// Copyright 13-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "data/tp3.h"

Tp3 *tp3_mk (void *e1, void *e2, void *e3) {
  Tp3 *this = MALLOC(Tp3);
  this->e1 = e1;
  this->e2 = e2;
  this->e3 = e3;
  return this;
}
