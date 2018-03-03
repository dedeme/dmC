// Copyright 26-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "writers/restore.h"
#include "dmc/all.h"

void restore_write(RW *rw, Structure *st) {
  if (st->head->mod != HEAD_SERIAL) {
    return;
  }

  rw_writeln(rw, "");
  rw_writeln(rw, str_printf(
    "%s *%srestore(Json *s) {", st->head->id, st->head->prefix
  ));
  rw_writeln(rw, "  Arr/*Json*/ *serial = json_rarray(s);");
  rw_writeln(rw, str_printf(
    "  %s *this = MALLOC(%s);", st->head->id, st->head->id
  ));
  rw_writeln(rw, "  size_t i = 0;");

  EACH(st->body->ps, Param, p) {
    char *s = p->serial;
    char *j = "";
    if (!strcmp(s, "_array")) j = "jarr_garray";
    else if (!strcmp(s, "_bool")) j = "jarr_gbool";
    else if (str_starts(s, "_double")) j = "jarr_gdouble";
    else if (!strcmp(s, "_int")) j = "jarr_gint";
    else if (!strcmp(s, "_object")) j = "jarr_gobject";
    else if (!strcmp(s, "_string")) j = "jarr_gstring";
    else if (!strcmp(s, "_uint")) j = "jarr_guint";

    if (!*j) {
      rw_writeln(rw, str_printf(
        "  this->%s = %s_restore(arr_get(serial, i++));", p->id, s
      ));
    } else {
      rw_writeln(rw, str_printf("  this->%s = %s(serial, i++);", p->id, j ));
    }
  }_EACH

  rw_writeln(rw, "  return this;");
  rw_writeln(rw, "}");
}
