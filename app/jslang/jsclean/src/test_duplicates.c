// Copyright 31-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "test_duplicates.h"
#include "io.h"

bool test_duplicates_run(Achar *roots) {
  int size = achar_size(roots);
  int i, j;
  for (i = 0; i < size - 1; ++i) {
    char *r1 = achar_get(roots, i);
    int len = str_len(r1) + 1;
    for (j = i + 1; j < size; ++j) {
      bool exists = false;
      char *r2 = achar_get(roots, j);

//    io.scan(r1,
        void fn(char *f) {
          char *relative = str_sub_end(f, len);
          char *ftest = path_cat(r2, relative, NULL);
          if (file_exists(ftest)) {
            printf(
              "File '%s' is duplicate:\n%s\n%s\n",
              path_name(f), f, ftest
            );
            exists = true;
          }
        }
//    );
      io_scan(r1, fn);

      if (exists) {
        return true;
      }
    }
  }
  return false;
}
