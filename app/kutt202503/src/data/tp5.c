// Copyright 14-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "data/tp5.h"

Tp5 *tp5_mk (void *e1, void *e2, void *e3, void *e4, void *e5) {
  Tp5 *this = MALLOC(Tp5);
  this->e1 = e1;
  this->e2 = e2;
  this->e3 = e3;
  this->e4 = e4;
  this->e5 = e5;
  return this;
}
