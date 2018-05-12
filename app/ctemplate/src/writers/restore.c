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
    "%s *%srestore(Arr/*Json*/ *serial) {", st->head->id, st->head->prefix
  ));
  rw_writeln(rw, "  if (!arr_size(serial)) return NULL;");
  rw_writeln(rw, str_printf(
    "  %s *this = MALLOC(%s);", st->head->id, st->head->id
  ));
  rw_writeln(rw, "  size_t i = 0;");

  EACH(st->body->ps, Param, p) {
    char *s = p->serial;
    char *j = "";
    if (str_starts(s, "_array")) j = str_printf("a%s", s + 6);
    else if (!strcmp(s, "_sarray")) j = "A";
    else if (!strcmp(s, "_bool")) j = "jarr_gbool";
    else if (str_starts(s, "_double")) j = "jarr_gdouble";
    else if (!strcmp(s, "_int")) j = "jarr_gint";
    else if (str_starts(s, "_object")) j = str_printf("o%s", s + 7);
    else if (!strcmp(s, "_sobject")) j = "O";
    else if (!strcmp(s, "_string")) j = "jarr_gstring";
    else if (!strcmp(s, "_uint")) j = "jarr_guint";

    if (!*j) {
      rw_writeln(rw, str_printf(
        "  this->%s = %s_restore(json_rarray(arr_get(serial, i++)));", p->id, s
      ));
    } else if (*j == 'A') {
      rw_writeln(rw, str_printf(
        "  this->%s = jarr_garray"
        "(serial, i++, (void*(*)(Json*))json_rstring);",
        p->id
      ));
    } else if (*j == 'O') {
      rw_writeln(rw, str_printf(
        "  this->%s = jarr_gobject"
        "(serial, i++, (void*(*)(Json*))json_rstring);",
        p->id
      ));
    } else if (*j == 'a') {
      rw_writeln(rw, str_printf(
        "  this->%s = jarr_garray"
        "(serial, i++, (void*(*)(Json*))%s_restore);",
        p->id, j + 1
      ));
    } else if (*j == 'o') {
      rw_writeln(rw, str_printf(
        "  this->%s = jarr_gobject"
        "(serial, i++, (void*(*)(Json*))%s_restore);",
        p->id, j + 1
      ));
    } else {
      rw_writeln(rw, str_printf("  this->%s = %s(serial, i++);", p->id, j ));
    }
  }_EACH

  rw_writeln(rw, "  return this;");
  rw_writeln(rw, "}");
}
