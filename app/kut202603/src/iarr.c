// Copyright 12-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "iarr.h"

struct iarr_Iarr {
  int *ints;
  int size;
};

Iarr *iarr_new (int n, int *ints) {
  if (n < 0)
    EXC_ILLEGAL_ARGUMENT(
      "Bad number of parameters", "value > 0", str_f("value == %d", n)
    );
  Iarr *this = MALLOC(Iarr);
  this->ints = ints;
  this->size = n;
  return this;
}

Iarr *iarr_new_from(int n, ...) {
  if (n < 0)
    EXC_ILLEGAL_ARGUMENT(
      "Bad number of parameters", "value > 0", str_f("value == %d", n)
    );
  int *ints = ATOMIC(n * sizeof(int));
  int *p = ints;

  va_list args;
  va_start(args, n);
  int ix;
  for (ix = 1; ix < n; ++ix) *p++ = va_arg(args, int);
  va_end(args);

  Iarr *this = MALLOC(Iarr);
  this->size = 0;
  this->ints = ints;
  return this;
}

int iarr_size (Iarr *this) {
  return this->size;
}

int iarr_get (Iarr *this, int ix) {
  EXC_RANGE(ix, 0, this->size - 1);
  return (this->ints)[ix];
}
