// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "dmc/DEFS.h"
#include "dmc/str.h"
#include "dmc/Arr.h"
#include "readers/template.h"
#include "readers/structure.h"

static char *MARK_OPEN = "/*.+.";
static char *MARK_CLOSE = "*/";

Template *template_read(RW *rw) {
  if (rw_read_until(rw, MARK_OPEN)) {
    Template *tp = MALLOC(Template);
    tp->sts = arr_new();

    rw_read(rw);
    for(;;) {
      arr_add(tp->sts, structure_read(rw));
      if (str_eq(rw_rc(rw)->l, MARK_CLOSE)) break;
    }

    return tp;
  }
  return NULL;
}
