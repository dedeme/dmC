// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdbool.h>
#include "dmc/str.h"
#include "dmc/Arr.h"
#include "dmc/DEFS.h"
#include "writers/structure.h"

void struct_write(RW *rw, Structure *st) {
  rw_writeln(rw, str_printf("struct %s%s {", st->head->prefix, st->head->id));

  bool first = true;
  bool exists = false;
  EACH(st->body->ps, Param, p) {
    if (first) {
      first = false;
      exists = true;
    }
    if (p->func) {
      rw_writeln(rw, str_printf(
        "  %s;",
        str_replace(p->type, "(*)", str_printf("(*%s)", p->id))
      ));
    } else {
      rw_writeln(rw, str_printf("  %s%s;", p->type, p->id));
    }
  }_EACH

  first = true;
  EACH(st->body->vs, Var, v) {
    if (first) {
      if (exists) rw_writeln(rw, "");
      first = false;
      exists = true;
    }
    rw_writeln(rw, str_printf("  %s%s;", v->type, v->id));
  }_EACH

  first = true;
  EACH(st->body->fs, Func, f) {
    if (first) {
      if (exists) rw_writeln(rw, "");
      first = false;
    }
    rw_writeln(rw, str_printf(
      "  %s;",
      str_replace(f->type, "(*)", str_printf("(*%s)", f->id))
    ));
  }_EACH

  rw_writeln(rw, "};");
}
