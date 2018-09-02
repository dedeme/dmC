// Copyright 31-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "filter.h"
#include "io.h"
#include <sys/stat.h>

Ap *filter_run(Achar *roots, char *target) {
  Ap *r = ap_new();

  EACH(roots, char, root) {
    int len = str_len(root) + 1;
//  io_scan(root,
      void fn(char *f) {
        if (!str_eq(path_extension(f), ".js")) {
          return;
        }
        char *relative = str_sub_end(f, len);
        char *ftest = path_cat(target, relative, NULL);
        if (!file_exists(ftest)) {
          ap_add(r, p_new(f, relative));
          return;
        }
        time_t f_time = file_info(f)->st_mtime;
        time_t ftest_time = file_info(ftest)->st_mtime;
        if (f_time > ftest_time) {
          ap_add(r, p_new(f, relative));
        }
      }
//  );
    io_scan(root, fn);
  }_EACH

  return r;
}
