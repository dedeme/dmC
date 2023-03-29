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
  if (fix != -1) {
    char *new_path = path_cat(path_parent(arr_get(paths,  fix)), ipath, NULL);
    if (file_exists(path_cat(root, str_f("%s.kut", new_path), NULL))) {
      ipath = new_path;
    }
  }
  if (!file_exists(path_cat(root, str_f("%s.kut", ipath), NULL))) return -1;

  EACH(paths, char, p) {
    if (str_eq(p, ipath)) return _i;
  }_EACH

  arr_push(paths, ipath);
  return arr_size(paths) - 1;
}

char *fileix_to_str (int ix) {
  if (ix < 0) return "Built-in";
  char *s = path_cat(root, arr_get(paths, ix), NULL);
  if (strlen(s) > 50) s = str_f("...%s", str_right(s, -47));
  return s;
}

char *fileix_to_fail (int ix) {
  if (ix < 0) return "Built-in";
  char *s = path_cat(root, arr_get(paths, ix), NULL);
  if (*s != '/')  s = path_cat(file_wd(), s, NULL);

  s = str_f("%s.kut", s);
  char *r = opt_get(path_canonical(s));
  if (!r) r = s;
  return r;
}

/// Read the file with index 'ix'
/// <char>
char *fileix_read (int ix) {
  char *p = path_cat(root, arr_get(paths, ix), NULL);
  return file_read(str_f("%s.kut", p));
}

