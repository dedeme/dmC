// Copyright 10-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader.h"
#include "Conf.h"

Opt *reader_book(void) {
  char *fjs = "book.js";
  if (!file_exists(fjs)) {
    puts("File 'book.js' not found");
    return opt_empty();
  }
  char *code = str_trim(file_read(fjs));

  Buf *bf = buf_new();
  char *rune;
  code = str_next_rune(&rune, code);
  while (*rune) {
    if (!rune[1] && *rune < ' ') buf_cadd(bf, ' ');
    else buf_add(bf, rune);
    code = str_next_rune(&rune, code);
  }
  code = buf_to_str(bf);

  while (str_index(code, "  ") != -1) code = str_replace(code, "  ", " ");
  if (!str_starts(code, "const book = {")) {
    puts("'book.js' does not start with 'const book = {'");
    return opt_empty();
  }

  Js *js = (Js *)str_sub(code, 13, -1);
  // Map[js]
  Map *book = js_ro(js);
  // Map[js]
  Map *jsunits = js_ro(opt_eget(
    map_get(book,"units"), "'units' field is missing"
  ));
  // Map[ConfUnit]
  Map *units = map_new();
  EACH((Arr *)jsunits, Kv, kv)
    char *uname = kv_key(kv);

    // Map[Js]
    Map *jsunit = js_ro(kv_value(kv));

    // Arr[Js]
    Arr *jsexes = js_ra(opt_eget(
      map_get(jsunit, "exercises"),
      str_f("'exercises' field is missing in unit '%s'", uname)
    ));

    // Arr[ConfExe]
    Arr *exes = arr_new();
    EACH(jsexes, Js, exe)
      // Arr[Js]
      Arr *jspgnm = js_ra(exe);
      arr_push(exes, confExe_new(
        js_ri(arr_get(jspgnm, 0)),
        js_ri(arr_get(jspgnm, 1))
      ));
    _EACH

    map_put(units, uname, confUnit_new(
      js_rs(opt_eget(
        map_get(jsunit, "name"),
        str_f("'name' field is missing in unit '%s'", uname)
      )),
      exes
    ));
  _EACH

  return opt_new(confBook_new(
    js_rs(opt_eget(map_get(book, "shortName"), "'shortName' field is missing")),
    js_rs(opt_eget(map_get(book, "longName"), "'longName' field is missing")),
    units
  ));
}
