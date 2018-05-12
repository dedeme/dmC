// Copyright 26-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "writers/serial.h"
#include "dmc/all.h"

void serial_write(RW *rw, Structure *st) {
  if (st->head->mod == HEAD_NO_SERIAL) {
    return;
  }

  rw_writeln(rw, "");
  rw_writeln(rw, str_printf(
    "Arr/*Json*/ *%sserialize(%s *this) {", st->head->prefix, st->head->id
  ));
  rw_writeln(rw, "  Arr/*Json*/ *serial = arr_new();");
  rw_writeln(rw, "  if (!this) return serial;");

  EACH(st->body->ps, Param, p) {
    char *s = p->serial;
    char *j = "";
    if (str_starts(s, "_array")) j = str_printf("a%s", s + 6);
    else if (!strcmp(s, "_sarray")) j = "A";
    else if (!strcmp(s, "_bool")) j = "jarr_abool";
    else if (str_starts(s, "_double")) j = str_printf("d%s", s + 7);
    else if (!strcmp(s, "_int")) j = "jarr_aint";
    else if (str_starts(s, "_object")) j = str_printf("o%s", s + 7);
    else if (!strcmp(s, "_sobject")) j = "O";
    else if (!strcmp(s, "_string")) j = "jarr_astring";
    else if (!strcmp(s, "_uint")) j = "jarr_auint";

    if (!*j) {
      rw_writeln(rw, str_printf(
        "  arr_add(serial, json_warray(%s_serialize(this->%s)));", s, p->id
      ));
    } else if (*j == 'd') {
      rw_writeln(rw, str_printf(
        "  jarr_adouble(serial, this->%s, %s);", p->id, j + 1
      ));
    } else if (*j == 'A') {
      rw_writeln(rw, str_printf(
        "  jarr_aarray(serial, this->%s, (Json*(*)(void*))json_wstring);",
        p->id
      ));
    } else if (*j == 'O') {
      rw_writeln(rw, str_printf(
        "  jarr_aobject(serial, this->%s, (Json*(*)(void*))json_wstring);",
        p->id
      ));
    } else if (*j == 'a') {
      rw_writeln(rw, str_printf(
        "  jarr_aarray(serial, this->%s, (Json*(*)(void*))%s_serialize);",
        p->id, j + 1
      ));
    } else if (*j == 'o') {
      rw_writeln(rw, str_printf(
        "  jarr_aobject(serial, this->%s, (Json*(*)(void*))%s_serialize);",
        p->id, j + 1
      ));
    } else {
      rw_writeln(rw, str_printf("  %s(serial, this->%s);", j, p->id ));
    }
  }_EACH

  rw_writeln(rw, "  return serial;");
  rw_writeln(rw, "}");
}
