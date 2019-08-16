// Copyright 06-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "io_tests.h"
#include <assert.h>
#include "io.h"

void io_tests (void) {
  puts("'io' tests:");

  Kv *_ps =params_read(
    3, (char *[]){"", "resources/lib", "resources/www"}
  );
  assert(!*kv_key(_ps));
  Params *ps = kv_value(_ps);
  assert(arr_size(params_roots(ps)) == 1);
  assert(str_eq(arr_get(params_roots(ps),0), "resources/lib"));
  assert(str_eq(params_target(ps), "resources/www"));
  assert(!*io_dir_exists(ps));

  _ps =params_read(
    3, (char *[]){"", "resources/dm", "resources/www"}
  );
  assert(!*kv_key(_ps));
  ps = kv_value(_ps);
  assert(str_eq(io_dir_exists(ps), "'resources/dm' is not a directory"));

  _ps =params_read(
    3, (char *[]){"", "resources/lib/dm", "www"}
  );
  assert(!*kv_key(_ps));
  ps = kv_value(_ps);
  assert(str_eq(io_dir_exists(ps), "'www' is not a directory"));

  _ps =params_read(
    3, (char *[]){"", "resources/lib/dm:wg", "resources/www"}
  );
  assert(!*kv_key(_ps));
  ps = kv_value(_ps);
  assert(str_eq(io_dir_exists(ps), "'wg' is not a directory"));

  file_cd("resources");

  _ps =params_read(
    3, (char *[]){"", "lib", "www"}
  );
  assert(!*kv_key(_ps));
  ps = kv_value(_ps);
  assert(!*io_dir_exists(ps));

  Arr *paths = io_all_files(ps);
  assert(arr_size(paths) == 7);
//  EACH(paths, Path, p) {
//    printf("[%s, %s]\n", path_absolute(p), path_relative(p));
//  }_EACH

  file_cd("..");

  puts("    Finished");
}

