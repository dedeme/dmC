// Copyright 13-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "data/tp.h"

Tp *tp_mk (void *e1, void *e2) {
  Tp *this = MALLOC(Tp);
  this->e1 = e1;
  this->e2 = e2;
  return this;
}
