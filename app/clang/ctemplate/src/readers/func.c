// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "readers/var.h"
#include "dmc/DEFS.h"
#include "dmc/str.h"
#include "dmc/exc.h"

Func *func_read(RW *rw) {
  Rcode *rc = rw_rc(rw);
  enum func_Mod mod = FUNC_GET;
  char *l = rc->l1;
  switch (rc->first) {
  case '-':
    mod = FUNC_PRIVATE;
    break;
  case '+':
    mod = FUNC_SET;
    break;
  default:
    l = rc->l;
  }

  Tp/*char, char*/ *tp = rw_split(rw, l, ':');
  Func *f = MALLOC(Func);
  f->mod = mod;
  f->id = tp_e1(tp);
  f->type = tp_e2(tp);
  if (str_index(f->type, "(*)") == -1)
    THROW(exc_illegal_state_t) rw_msg(rw, "'(*)' is missing") _THROW

  rw_read(rw);
  return f;
}
