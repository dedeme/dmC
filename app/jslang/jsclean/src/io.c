// Copyright 06-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "io.h"

char *io_dir_exists (Params *ps) {
  char *r = "";
  void fn (char *path) {
    if (!file_is_directory(path)) {
      r = str_f("'%s' is not a directory", path);
    }
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

Arr *io_filter_lint (Arr *paths, char *target) {
  char *cache_path = path_cat(sys_home(), "cache.db", NULL);
  // Arr[char]
  Arr *cache_files = file_exists(cache_path)
    ? arr_from_js((Js *)file_read(cache_path), (FFROM)js_rs)
    : arr_new()
  ;
  // Arr[char]
  Arr *target_files = arr_new();
  void ftarget_files (char *dir) {
    EACH(file_dir(dir), char, name) {
      char *file = path_cat(dir, name, NULL);
      if (file_is_directory(file)) {
        ftarget_files(file);
      } else {
        arr_push(target_files, file);
      }
    }_EACH
  }
  if (file_is_directory(target)) {
    ftarget_files(target);
  }

  int filter_cache (char *f) {
    EACH(paths, Path, p) {
      if (str_eq(path_absolute(p), f)) {
        return 1;
      }
    }_EACH
    return 0;
  }

  int filter_js (Path *p) {
    char *target_file = path_cat(target, path_relative(p), NULL);
    return file_exists(target_file)
      ? file_modified(path_absolute(p)) > file_modified(target_file)
      : 1
    ;
  }

  // Arr[char]
  Arr *tmpr = it_to(it_cat(
    it_filter(it_from(cache_files), (FPRED)filter_cache),
    it_map(it_filter(it_from(paths), (FPRED)filter_js), (FCOPY)path_absolute)
  ));

  return tp_e2(arr_duplicates(tmpr, (FCMP)str_eq));
}

// 'files' is Arr[char]
void io_write_cache (Arr *files) {
  char *cache_path = path_cat(sys_home(), "cache.db", NULL);
  file_write(cache_path, (char *)arr_to_js(files, (FTO)js_ws));
}

void io_copy_js (Arr *paths, char *target) {
  file_del(target);
  file_mkdir(target);

  EACH(paths, Path, p) {
    char *t = path_cat(target, path_relative(p), NULL);
    if (!file_exists(path_parent(t))) {
      file_mkdir(path_parent(t));
    }
    file_copy(path_absolute(p), t);
  }_EACH
}
