// Copyright 06-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/Path.h"

/* .
Path
  absolute: char *
  relative: char *
*/
/*--*/

struct Path_Path {
  char *absolute;
  char *relative;
};

Path *path_new (char *absolute, char *relative) {
  Path *this = MALLOC(Path);
  this->absolute = absolute;
  this->relative = relative;
  return this;
}

char *path_absolute (Path *this) {
  return this->absolute;
}

char *path_relative (Path *this) {
  return this->relative;
}

/*--*/

// Arr[Path]
Arr *path_duplicates (Arr *paths) {
  int feq (Path *p1, Path *p2) {
    return str_eq(path_relative(p1), path_relative(p2));
  }

  return tp_e1(it_duplicates(it_from(paths), (FCMP)feq));
}
