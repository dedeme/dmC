// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "readers/var.h"
#include "dm/dm.h"

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
  f->id = tp->e1;
  f->type = tp->e2;
  if (str_index(f->type, "(*)") == -1)
    THROW rw_msg(rw, "'(*)' is missing") _THROW

  rw_read(rw);
  return f;
}
