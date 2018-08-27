// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "io.h"
#include "types/Cpath.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/file.h"
#include "dmc/DEFS.h"

Either/*Cpath*/ *io_cpath(Lchar *roots, char *relative_path) {
  Cpath *cpath = NULL;

  EACHL(roots, char, root) {
    Cpath *cp = cpath_new(root, relative_path);
    if (file_exists(cpath_complete(cp))) {
      if (cpath) {
        return either_fail(str_printf(
          "Path '%s' is duplicate in '%s' and '%s'.",
          relative_path, cpath_complete(cpath), cpath_complete(cp)
        ));
      }
      cpath = cp;
    }
  }_EACH

  if (cpath) {
    return either_ok(cpath);
  }

  return either_fail(str_printf("Path '%s' not found.", relative_path));
}

Ojson *io_read_class(Cpath *path) {
  THROW("") "Unimplemented" _THROW;
  return ojson_null();
}
