// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "readers/head.h"
#include "dmc/all.h"
#include "ctype.h"

Head *head_read(RW *rw) {
  Rcode *rc = rw_rc(rw);
  if (rc->indent)
    THROW rw_msg(rw, "Indented structure head") _THROW

  char *l = rc->l;
  enum head_Mod mod = HEAD_SERIAL;
  if (rc->first == '-') {
    mod = HEAD_NO_SERIAL;
    l = rc->l1;
  }

  Tp/*char, char*/ *tp = rw_split(rw, l, ':');

  if (strcmp(tp->e1, "struct"))
    THROW rw_msg(rw, "Expected 'struct'") _THROW
  char *id = tp->e2;

  enum head_Id_mod id_mod = HEAD_ID_PUBLIC;
  if (*id == '-') {
    id_mod = HEAD_ID_PRIVATE;
    id = str_sub_end(id, 1);
  }
  if (!*id)
    THROW rw_msg(rw, "Id is missing") _THROW

  Head *h = MALLOC(Head);
  h->mod = mod;
  h->id_mod = id_mod;
  h->id = id;
  h->prefix = str_printf("%s_", id);
  *h->prefix = tolower(*h->prefix);

  rw_read(rw);
  return h;
}
