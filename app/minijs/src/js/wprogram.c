// Copyright 16-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "js/wprogram.h"

void wprogram_write(Cpath *source, char *target) {
  char *sdir = path_parent(cpath_js(source));
  LckFile *lck = file_wopen(target);
  EACH(file_dir(sdir), char, f) {
    if (str_ends(f, ".js")) {
      file_write_text(lck, file_read(f));
      file_write_text(lck, "\n");
    }
  }_EACH
  file_write_text(lck, str_printf("%s.main();\n", cpath_id(source)));
  file_close(lck);
}
