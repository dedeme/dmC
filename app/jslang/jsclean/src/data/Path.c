// Copyright 06-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/Path.h"

/* .
Path: serial
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

Js *path_to_js (Path *this) {
  // Arr[Js]
  Arr *js = arr_new();
  arr_push(js, js_ws(this->absolute));
  arr_push(js, js_ws(this->relative));
  return js_wa(js);
}

Path *path_from_js (Js *js) {
  // Arr[Js]
  Arr *a = js_ra(js);
  Js **p = (Js **)arr_start(a);
  Path *this = MALLOC(Path);
  this->absolute = js_rs(*p++);
  this->relative = js_rs(*p++);
  return this;
}

/*--*/

// Arr[Path]
Arr *path_duplicates (Arr *paths) {
  int feq (Path *p1, Path *p2) {
    return str_eq(path_relative(p1), path_relative(p2));
  }

  return tp_e1(it_duplicates(it_from(paths), (FCMP)feq));
}

int path_eq(Path *p1, Path *p2) {
  return str_eq(p1->relative, p2->relative);
}
