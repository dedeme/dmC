// Copyright 06-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "io.h"

char *io_dir_exists (Params *ps) {
  char *r = "";
  void fn (char *path) {
    if (!file_is_directory(path))
      r = str_f("'%s' is not a directory", path);
    if (*path == '/')
      r = str_f("'%s' an absolute path", path);
  }
  it_each(it_from(params_roots(ps)), (FPROC)fn);
  return r;
}

// Arr[Path]
Arr *io_all_files (Params *ps) {
  //Arr[Path]
  Arr *r = arr_new();

  EACH(params_roots(ps), char, root) {
    int cut = strlen(root) + 1;
    void fn (char *dir) {
      EACH(file_dir(dir), char, name) {
        char *file = path_cat(dir, name, NULL);
        if (file_is_directory(file)) {
          fn(file);
        } else {
          if (str_ends(name, ".js")) {
            arr_push(r, path_new(file, str_right(file, cut)
            ));
          }
        }
      }_EACH
    }
    fn(root);
  }_EACH

  return r;
}

// Arr<Path>. paths is Arr<Path>
Arr *io_filter_lint (Arr *paths, char *target) {
  int cut = strlen(target) + 1;
  // Arr<char>
  Arr *ts = arr_new();
  void ts_add (char *dir) {
    EACH(file_dir(dir), char, f) {
      char *f2 = path_cat(dir, f, NULL);
      if (file_is_directory(f2)) {
        ts_add(f2);
      } else {
        if (str_ends(f2, ".js"))
          arr_push(ts, path_new(f2, str_right(f2, cut)));
      }
    }_EACH
  }
  ts_add(target);

  EACH(ts, Path, p) {
    int fn (Path *pth) { return path_eq(p, pth); }
    if (arr_index(paths, (FPRED)fn) == -1) {
      file_del(path_absolute(p));
    }
  }_EACH
  EACH(ts, Path, p) {
    char *f = path_absolute(p);
    if (file_is_directory(f) && !arr_size(file_dir(f)))
      file_del(f);
  }_EACH

  char *cache = path_cat(sys_home(), "cache.db", NULL);
  // Arr[Path]
  Arr *cache_paths = file_exists(cache)
    ? arr_from_js((Js *)file_read(cache), (FFROM)path_from_js)
    : arr_new()
  ;

  // Arr<Path>
  Arr *r = arr_new();
  EACH(cache_paths, Path, cp) {
    int fn (Path *p) { return path_eq(cp, p); }
    if (arr_index(paths, (FPRED)fn) != -1)
      arr_push(r, cp);
  }_EACH

  EACH(paths, Path, p) {
    int fn (Path *pth) { return path_eq(pth, p); }
    if (arr_index(r, (FPRED)fn) != -1)
      continue;

    int add = 1;
    EACH(ts, Path, pt) {
      if (
        str_eq(path_relative(pt), path_relative(p)) &&
        file_modified(path_absolute(p)) < file_modified(path_absolute(pt))
      ) {
        add = 0;
        break;
      }
    }_EACH
    if (add) arr_push(r, p);
  }_EACH

  return r;
}

// 'paths' is Arr[Path]
void io_write_cache (Arr *paths) {
  char *cache_path = path_cat(sys_home(), "cache.db", NULL);
  file_write(cache_path, (char *)arr_to_js(paths, (FTO)path_to_js));
}

// 'paths' is Arr<Path>
void io_copy_js (Arr *paths, char *target) {
  EACH(paths, Path, p) {
    puts(path_relative(p));
    char *ft = path_cat(target, path_relative(p), NULL);
    file_mkdir(path_parent(ft));
    file_copy(path_absolute(p), ft);
  }_EACH
}
