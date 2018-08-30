// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "readers/var.h"
#include "dmc/DEFS.h"
#include "dmc/str.h"

Var *var_read(RW *rw) {
  Rcode *rc = rw_rc(rw);
  enum var_Mod mod = VAR_GET;
  char *l = rc->l1;
  switch (rc->first) {
  case '-':
    mod = VAR_PRIVATE;
    break;
  case '+':
    mod = VAR_SET;
    break;
  default:
    l = rc->l;
  }

  Tp/*char, char*/ *tp = rw_split(rw, l, ':');
  Var *v = MALLOC(Var);
  v->mod = mod;
  v->id = tp_e1(tp);
  tp = rw_split(rw, tp_e2(tp), '=');
  v->type = tp_e1(tp);
  v->value = tp_e2(tp);

  if (v->type[str_len(v->type) - 1] != '*') {
    v->type = str_printf("%s ", v->type);
  }

  rw_read(rw);
  return v;
}
