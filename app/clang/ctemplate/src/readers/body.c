// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include <assert.h>
#include "dmc/Arr.h"
#include "dmc/str.h"
#include "dmc/DEFS.h"
#include "readers/param.h"
#include "readers/var.h"
#include "readers/func.h"

static char *MARK = "-";

Body *body_read(RW *rw, Head *head) {
  Body *b = MALLOC(Body);
  b->ps = arr_new();
  b->vs = arr_new();
  b->fs = arr_new();

  Rcode *rc = rw_rc(rw);
  for(;;) {
    if (!rc->indent) return b;
    if (str_eq(rc->l, MARK)) {
      rw_read(rw);
      rc = rw_rc(rw);
      break;
    }
    arr_add(b->ps, param_read(rw, head));
    rc = rw_rc(rw);
  }

  for(;;) {
    if (!rc->indent) return b;
    if (str_eq(rc->l, MARK)) {
      rw_read(rw);
      rc = rw_rc(rw);
      break;
    }
    arr_add(b->vs, var_read(rw));
    rc = rw_rc(rw);
  }

  for(;;) {
    if (!rc->indent) return b;
    arr_add(b->fs, func_read(rw));
    rc = rw_rc(rw);
  }

  assert(false);
  return NULL;
}
