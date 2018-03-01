// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "writers/typedef.h"
#include "dm/dm.h"

void typedef_write(RW *rw, Structure *st) {
  if (st->head->id_mod == HEAD_ID_PRIVATE) {
    rw_writeln(rw, "");
    rw_writeln(rw, str_printf(
      "typedef struct %s%s %s;",
      st->head->prefix, st->head->id, st->head->id
    ));
  }
}
