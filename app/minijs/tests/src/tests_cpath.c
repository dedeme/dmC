// Copyright 1-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_cpath.h"
#include "Cpath.h"

void tests_cpath() {
  puts("Cpath");

  Cpath *p1 = cpath_new("Empty");
  assert(!strcmp(cpath_path(p1), "Empty"));
  assert(!strcmp(cpath_fpath(p1), "Empty.mini"));
  assert(!strcmp(cpath_id(p1), "Empty"));
  assert(!strcmp(
    cpath_file(p1), path_cat(file_cwd(), "data", "Empty.mini", NULL)
  ));
  assert(!strcmp(cpath_parent(p1), path_cat(file_cwd(), "data", NULL)));
  assert(!strcmp(
    cpath_lib(p1),
    str_printf("%s/paths%s/data/Empty", sys_home(), file_cwd())
  ));
  assert(!strcmp(
    cpath_js(p1),
    str_printf("%s/paths%s/data/Empty.js", sys_home(), file_cwd())
  ));

  p1 = cpath_new("sub/Empty");
  assert(!strcmp(cpath_path(p1), "sub/Empty"));
  assert(!strcmp(cpath_fpath(p1), "sub/Empty.mini"));
  assert(!strcmp(cpath_id(p1), "sub_Empty"));
  assert(!strcmp(
    cpath_file(p1), path_cat(file_cwd(), "data", "sub", "Empty.mini", NULL)
  ));
  assert(!strcmp(cpath_parent(p1), path_cat(file_cwd(), "data", "sub", NULL)));
  assert(!strcmp(
    cpath_lib(p1),
    str_printf("%s/paths%s/data/sub/Empty", sys_home(), file_cwd())
  ));
  assert(!strcmp(
    cpath_js(p1),
    str_printf("%s/paths%s/data/sub/Empty.js", sys_home(), file_cwd())
  ));

  puts("    Finished");
}
