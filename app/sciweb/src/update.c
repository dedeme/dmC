// Copyright 10-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "update.h"
#include "Conf.h"
#include "reader.h"
#include "tpls/unit.h"
#include "tpls/exercise.h"

void update (void) {
  // Arr[char]
  Arr *msgs = arr_new();

  ConfBook *book = opt_nget(reader_book());
  if (book) {
    EACH((Arr *)confBook_units(book), Kv, kv)
      char *ukey = kv_key(kv);
      ConfUnit *unit = kv_value(kv);
      // Arr[ConfExe]
      Arr *exes = confUnit_exercises(unit);

      char *funit = str_f("%s.html", ukey);
      if (!file_exists(funit)) {
        file_write(funit, unit_html(ukey));
        arr_push(msgs, str_f("Creado '%s'", funit));
      }

      char *exedir = str_f("exercises/%s", ukey);
      if (!file_exists(exedir)) {
        file_mkdir(exedir);
        arr_push(msgs, str_f("Creado '%s'", exedir));
      }

      EACH(exes, ConfExe, exe)
        char *ekey = str_f(
          "p%03d-%03d", confExe_page(exe), confExe_number(exe)
        );
        char *fexe = str_f("exercises/%s/%s.html", ukey, ekey);
        if (!file_exists(fexe)) {
          file_write(fexe, exercise_html(ukey, ekey));
          arr_push(msgs, str_f("Creado '%s'", fexe));
        }
      _EACH
    _EACH
  }

  if (arr_size(msgs)) {
    puts(str_cjoin(msgs, '\n'));
  } else {
    puts("No update was done");
  }

}
