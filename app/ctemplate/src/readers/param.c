// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "readers/param.h"


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
  pr->id = tp_e1(tp);
  pr->type = tp_e2(tp);
  pr->serial = "";

  if (str_index(pr->type, "(*)") != -1) {
    pr->func = true;
  }

  if (head->mod == HEAD_SERIAL) {
    if (pr->func)
      THROW(exc_illegal_state_t)
        rw_msg(rw, "There can not be functions in a serializable structure")
      _THROW
    tp = rw_split(rw, tp_e2(tp), ':');
    pr->type = tp_e1(tp);
    pr->serial = tp_e2(tp);
  }

  if (!pr->func && pr->type[str_len(pr->type) - 1] != '*') {
    pr->type = str_printf("%s ", pr->type);
  }

  rw_read(rw);
  return pr;
}
