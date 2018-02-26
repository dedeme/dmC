// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "readers/param.h"
#include "dm.h"


Param *param_read(RW *rw, Head *head) {
  Rcode *rc = rw_rc(rw);

  enum param_Mod mod = PARAM_GET;
  char *l = rc->l1;
  switch (rc->first) {
  case '-':
    mod = PARAM_PRIVATE;
    break;
  case '+':
    mod = PARAM_SET;
    break;
  default:
    l = rc->l;
  }

  Tp/*char, char*/ *tp = rw_split(rw, l, ':');
  Param *pr = MALLOC(Param);
  pr->mod = mod;
  pr->func = false;
  pr->id = tp->e1;
  pr->type = tp->e2;
  pr->serial = "";

  if (str_index(pr->type, "(*)") != -1) {
    pr->func = true;
  }

  if (head->mod == HEAD_SERIAL || head->mod == HEAD_RESTORE) {
    if (pr->func)
      THROW
        rw_msg(rw, "There can not be functions in a serializable structure")
      _THROW
    tp = rw_split(rw, tp->e2, ':');
    pr->type = tp->e1;
    pr->serial = tp->e2;
  }

  if (!pr->func && pr->type[strlen(pr->type) - 1] != '*') {
    pr->type = str_printf("%s ", pr->type);
  }

  rw_read(rw);
  return pr;
}
