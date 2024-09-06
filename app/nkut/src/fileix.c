// Copyright 03-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "fileix.h"
#include "kut/DEFS.h"
#include "kut/path.h"
#include "kut/file.h"
#include "kut/opt.h"

static char *root = "";
// <char>
static Arr *paths;

static char *find(char *cpath, char *dir, char *file) {
  char *r = path_cat(dir, file, NULL);
  char *r_cpath = opt_get(path_canonical(r));
  if (r_cpath && !strcmp(r_cpath, cpath)) return r;
  EACH(file_dir(dir), char, f) {
    char *newd = path_cat(dir, f, NULL);
    if (file_is_directory(newd)) {
      r = find(cpath, newd, file);
      if (*r) return r;
    }
  }_EACH
  return "";
}

void fileix_init(char *froot) {
  paths = arr_new();
  froot = *froot ? froot : "./";
  root = opt_get(path_canonical(froot));
  if (!root)
    EXC_ILLEGAL_STATE("'froot' not found");
}

char *fileix_get_root (void) {
  return root;
}

int fileix_add(int fix, char *ipath) {
  char *new_path = opt_get(path_canonical(
    path_cat(root, str_f("%s.nkut", ipath), NULL)
  ));
  if (fix != -1) {
    char *opt_path = opt_get(path_canonical(str_f(
      "%s.nkut",
      path_cat(path_parent(arr_get(paths,  fix)), ipath, NULL
    ))));
    if (opt_path) {
      new_path = opt_path;
    }
  }
  if (!new_path) return -1;


  if (!str_starts(new_path, root)) {
    new_path = find(new_path, root, path_base(new_path));
    if (!*new_path) return -1;
  }

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
char *fileix_read (int ix) {
  return file_read(arr_get(paths, ix));
}

char *fileix_to_root (int ix) {
  char *p = str_right(arr_get(paths, ix), strlen(root) + 1);
  char *r = "";
  while (*p) {
    char ch = *p++;
    if (ch == '/') r = str_f("%s../", r);
  }
  return *r ? r : "./";
}

char *fileix_relative (int ix) {
  return str_right(arr_get(paths, ix), strlen(root) + 1);
}

