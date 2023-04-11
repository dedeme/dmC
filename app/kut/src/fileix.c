// Copyright 03-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "fileix.h"
#include "DEFS.h"
#include "kut/path.h"
#include "kut/file.h"
#include "kut/opt.h"

static char *root = "./";
static Arr *paths;

void fileix_init(void) {
  paths = arr_new();
}

void fileix_set_root (char *froot) {
  root = froot;
}

char *fileix_get_root (void) {
  return root;
}

int fileix_add(int fix, char *ipath) {
  char *new_path = opt_get(path_canonical(
    path_cat(root, str_f("%s.kut", ipath), NULL)
  ));
  if (fix != -1) {
    char *opt_path = opt_get(path_canonical(str_f(
      "%s.kut",
      path_cat(path_parent(arr_get(paths,  fix)), ipath, NULL
    ))));
    if (opt_path) {
      new_path = opt_path;
    }
  }
  if (!new_path) return -1;

  EACH(paths, char, p) {
    if (str_eq(p, new_path)) return _i;
  }_EACH

  arr_push(paths, new_path);
  return arr_size(paths) - 1;
}

char *fileix_to_str (int ix) {
  if (ix < 0) return "Built-in";
  char *s = str_left(arr_get(paths, ix), - 4);
  if (strlen(s) > 50) s = str_f("...%s", str_right(s, -47));
  return s;
}

char *fileix_to_fail (int ix) {
  if (ix < 0) return "Built-in";
  return arr_get(paths, ix);
}

/// Read the file with index 'ix'
/// <char>
char *fileix_read (int ix) {
  return file_read(arr_get(paths, ix));
}

