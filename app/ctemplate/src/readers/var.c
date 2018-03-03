// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "readers/var.h"
#include "dmc/all.h"

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
  v->id = tp->e1;
  tp = rw_split(rw, tp->e2, '=');
  v->type = tp->e1;
  v->value = tp->e2;

  if (v->type[strlen(v->type) - 1] != '*') {
    v->type = str_printf("%s ", v->type);
  }

  rw_read(rw);
  return v;
}
