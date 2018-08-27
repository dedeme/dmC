// Copyright 26-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "writers/restore.h"
#include "dmc/str.h"
#include "dmc/Arr.h"
#include "dmc/ct/Achar.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

void restore_write(RW *rw, Structure *st) {
  if (st->head->mod != HEAD_SERIAL) {
    return;
  }

  rw_writeln(rw, "");
  rw_writeln(rw, str_printf(
    "%s *%sfrom_json(Json *js) {", st->head->id, st->head->prefix
  ));
  rw_writeln(rw, "  XNULL(js)");
  rw_writeln(rw, "  Ajson *serial = json_rarray(js);");
  rw_writeln(rw, str_printf(
    "  %s *this = MALLOC(%s);", st->head->id, st->head->id
  ));
  rw_writeln(rw, "  size_t i = 0;");

  EACH(st->body->ps, Param, p) {
    char *s = p->serial;
    char *j = "";
    if (str_starts(s, "_array")) j = str_printf("a%s", s + 6);
    else if (str_eq(s, "_sarray")) j = "A";
    else if (str_eq(s, "_bool")) j = "jarr_gbool";
    else if (str_starts(s, "_double")) j = "jarr_gdouble";
    else if (str_eq(s, "_int")) j = "jarr_gint";
    else if (str_starts(s, "_object")) j = str_printf("o%s", s + 7);
    else if (str_eq(s, "_sobject")) j = "O";
    else if (str_eq(s, "_string")) j = "jarr_gstring";
    else if (str_eq(s, "_uint")) j = "jarr_guint";

    if (!*j) {
     if (str_starts(s, "__")) {
        Achar *ps = str_csplit_trim(s, '-');
        if (achar_size(ps) != 2)
          THROW(exc_range_t) exc_range(2, 3, achar_size(ps)) _THROW
        rw_writeln(rw, str_printf(
          "  this->%s = "
            "%s_from_json(ajson_get(serial, i++), %s_from_json);",
          p->id, str_sub_end(achar_get(ps, 0), 2), achar_get(ps, 1)
        ));
      } else {
        rw_writeln(rw, str_printf(
          "  this->%s = %s_from_json(ajson_get(serial, i++));",
          p->id, s
        ));
      }
    } else if (*j == 'A') {
      rw_writeln(rw, str_printf(
        "  this->%s = (Achar *)jarr_garray"
        "(serial, i++, (void*(*)(Json*))json_rstring);",
        p->id
      ));
    } else if (*j == 'O') {
      rw_writeln(rw, str_printf(
        "  this->%s = (Mchar *)jarr_gobject"
        "(serial, i++, (void*(*)(Json*))json_rstring);",
        p->id
      ));
    } else if (*j == 'a') {
      rw_writeln(rw, str_printf(
        "  this->%s = (%s)jarr_garray"
        "(serial, i++, (void*(*)(Json*))%s_from_json);",
        p->id, p->type, j + 1
      ));
    } else if (*j == 'o') {
      rw_writeln(rw, str_printf(
        "  this->%s = (%s)jarr_gobject"
        "(serial, i++, (void*(*)(Json*))%s_from_json);",
        p->id, p->type, j + 1
      ));
    } else {
      rw_writeln(rw, str_printf("  this->%s = %s(serial, i++);", p->id, j ));
    }
  }_EACH

  rw_writeln(rw, "  return this;");
  rw_writeln(rw, "}");
}
