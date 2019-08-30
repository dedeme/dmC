// Copyright 06-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "jsclean.h"
#include "data/Params.h"
#include "io.h"
#include "lint.h"

int main (int argc, char *argv[]) {
  sys_init("jsclean");

  // Kv<Params>. Key != "" is an error.
  Kv *_ps = params_read(argc, argv);
  if (*kv_key(_ps)) {
    puts(kv_key(_ps));
    return 1;
  }
  Params *ps = kv_value(_ps);

  // e is a error message
  char *e = io_dir_exists(ps);
  if (*e) {
    puts(e);
    return 1;
  }

  // Arr[Path]
  Arr *paths = io_all_files(ps);

  // Arr[Path]
  Arr *dup = path_duplicates(paths);
  if (arr_size(dup)) {
    puts ("Following files are duplicates:");
    EACH(paths, Path, p) {
      printf("%s -> %s\n", path_absolute(p), path_relative(p));
    }_EACH
    return 1;
  }

  // Arr[Path]
  Arr *lint_paths = io_filter_lint(paths, params_target(ps));

  e = lint_run(lint_paths);
  if (*e) {
    puts(e);
    io_write_cache(lint_paths);
  } else {
    io_write_cache(arr_new());
  }

  io_copy_js (lint_paths, params_target(ps));
}
