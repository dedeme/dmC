// Copyright 31-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "io.h"

void io_scan(char *dir, void (*fn)(char *)) {
  EACH (file_dir(dir), char, f) {
    if (file_is_directory(f)) {
      io_scan(f, fn);
    }
    fn(f);
  }_EACH
}
