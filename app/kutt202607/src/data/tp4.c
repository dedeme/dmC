// Copyright 13-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "data/tp4.h"

Tp4 *tp4_mk (void *e1, void *e2, void *e3, void *e4) {
  Tp4 *this = MALLOC(Tp4);
  this->e1 = e1;
  this->e2 = e2;
  this->e3 = e3;
  this->e4 = e4;
  return this;
}
