// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "writers/tmpc.h"
#include "dm/dm.h"
#include "writers/structure.h"

static char *MARK = "/*.-.*/";

void tmpc_write(Template *tp, RW *rw) {
  rw_read_until(rw, MARK);

  bool first = true;
  EACH(tp->sts, Structure, st) {
    if (first) first = false;
    else rw_writeln(rw, "");

    structure_write(rw, st);
  }_EACH

  rw_read_blank(rw, MARK);
  rw_read_end(rw);
}
