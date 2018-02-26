// Copyright 26-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "writers/serial.h"
#include "dm.h"

void serial_write(RW *rw, Structure *st) {
  if (st->head->mod == HEAD_NO_SERIAL) {
    return;
  }

  rw_writeln(rw, "");
  rw_writeln(rw, str_printf(
    "Json *%sserialize(%s *this) {", st->head->prefix, st->head->id
  ));
  rw_writeln(rw, "  Arr/*Json*/ *serial = arr_new();");

  EACH(st->body->ps, Param, p) {
    char *s = p->serial;
    char *j = "";
    if (!strcmp(s, "_array")) j = "jarr_aarray";
    else if (!strcmp(s, "_bool")) j = "jarr_abool";
    else if (!strcmp(s, "_double")) j = "jarr_adouble";
    else if (!strcmp(s, "_int")) j = "jarr_aint";
    else if (!strcmp(s, "_object")) j = "jarr_aobject";
    else if (!strcmp(s, "_string")) j = "jarr_astring";
    else if (!strcmp(s, "_uint")) j = "jarr_auint";

    if (!*j) {
      rw_writeln(rw, str_printf(
        "  arr_add(serial, %s_serialize(this->%s));", s, p->id
      ));
    } else {
      rw_writeln(rw, str_printf("  %s(serial, this->%s);", j, p->id ));
    }
  }_EACH

  rw_writeln(rw, "  return json_warray(serial);");
  rw_writeln(rw, "}");
}
