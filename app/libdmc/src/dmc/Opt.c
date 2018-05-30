// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include "dmc/Opt.h"
#include "dmc/DEFS.h"
#include "dmc/exc.h"
#include "dmc/str.h"

inline
Opt *opt_null() {
  return (Opt *)NULL;
}

Opt *opt_new(void *v) {
  XNULL(v)
  return v;
}

inline
bool opt_is_null(Opt *this) {
  return !this;
}

void *opt_value(Opt *this) {
  XNULL(this)
  return this;
}
