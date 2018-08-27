// Copyright 26-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "writers/serial.h"
#include "dmc/str.h"
#include "dmc/Arr.h"
#include "dmc/ct/Achar.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

void serial_write(RW *rw, Structure *st) {
  if (st->head->mod == HEAD_NO_SERIAL) {
    return;
  }

  rw_writeln(rw, "");
  rw_writeln(rw, str_printf(
    "Json *%sto_json(%s *this) {", st->head->prefix, st->head->id
  ));
  rw_writeln(rw, "  XNULL(this)");
  rw_writeln(rw, "  Ajson *serial = ajson_new();");

  EACH(st->body->ps, Param, p) {
    char *s = p->serial;
    char *j = "";
    if (str_starts(s, "_array")) j = str_printf("a%s", s + 6);
    else if (str_eq(s, "_sarray")) j = "A";
    else if (str_eq(s, "_bool")) j = "jarr_abool";
    else if (str_starts(s, "_double")) j = str_printf("d%s", s + 7);
    else if (str_eq(s, "_int")) j = "jarr_aint";
    else if (str_starts(s, "_object")) j = str_printf("o%s", s + 7);
    else if (str_eq(s, "_sobject")) j = "O";
    else if (str_eq(s, "_string")) j = "jarr_astring";
    else if (str_eq(s, "_uint")) j = "jarr_auint";

    if (!*j) {
      if (str_starts(s, "__")) {
        Achar *ps = str_csplit_trim(s, '-');
        if (achar_size(ps) != 2)
          THROW(exc_range_t) exc_range(2, 3, achar_size(ps)) _THROW
        rw_writeln(rw, str_printf(
          "  ajson_add(serial, %s_to_json(this->%s, %s_to_json));",
          str_sub_end(achar_get(ps, 0), 2), p->id, achar_get(ps, 1)
        ));
      } else {
        rw_writeln(rw, str_printf(
          "  ajson_add(serial, %s_to_json(this->%s));", s, p->id
        ));
      }
    } else if (*j == 'd') {
      rw_writeln(rw, str_printf(
        "  jarr_adouble(serial, this->%s, %s);", p->id, j + 1
      ));
    } else if (*j == 'A') {
      rw_writeln(rw, str_printf(
        "  jarr_aarray"
        "(serial, (Arr *)this->%s, (Json*(*)(void*))json_wstring);",
        p->id
      ));
    } else if (*j == 'O') {
      rw_writeln(rw, str_printf(
        "  jarr_aobject"
        "(serial, (Map *)this->%s, (Json*(*)(void*))json_wstring);",
        p->id
      ));
    } else if (*j == 'a') {
      rw_writeln(rw, str_printf(
        "  jarr_aarray(serial, (Arr *)this->%s, (Json*(*)(void*))%s_to_json);",
        p->id, j + 1
      ));
    } else if (*j == 'o') {
      rw_writeln(rw, str_printf(
        "  jarr_aobject(serial, (Map *)this->%s, (Json*(*)(void*))%s_to_json);",
        p->id, j + 1
      ));
    } else {
      rw_writeln(rw, str_printf("  %s(serial, this->%s);", j, p->id ));
    }
  }_EACH

  rw_writeln(rw, "  return json_warray(serial);");
  rw_writeln(rw, "}");
}
