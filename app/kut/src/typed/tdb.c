// Copyright 06-Sep-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "typed/tdb.h"
#include "kut/file.h"
#include "kut/path.h"
#include "kut/sys.h"
#include "kut/js.h"
#include "kut/opt.h"

static void clean_scripts (char *root, Map *m) {
  EACH(map_keys(m), char, k) {
    if (!file_exists(k)) {
      char *v = opt_get(map_get(m, k));
      if (!v) EXC_KUT("Key does not exists");
      file_del(path_cat(root, str_f("%s.so", v), NULL));
      map_remove(m, k);
    }
  }_EACH
}

void tdb_init (char *path) {
  char *version_tx = str_f("%s\n", VERSION);
  char *root = path_cat(sys_user_home(), ".dmCApp", "kut", NULL);
  char *version_path = path_cat(root, "version", NULL);
  char *scripts_path = path_cat(root, "scripts.tb", NULL);
  char *tmp_path = path_cat(root, "tmp.c", NULL);

  if (!file_exists(root)) {
    file_mkdir(root);
    file_write(version_path, version_tx);
    file_write(scripts_path, "[0,{}]");
    file_write(tmp_path, "");
  }
  if (strcmp(file_read(version_path), version_tx)) {
    file_write(version_path, version_tx);
    file_write(scripts_path, "[0,{}]");
  }
}

char *tdb_c_path () {
  return path_cat(sys_user_home(), ".dmCApp", "kut", "tmp.c", NULL);
}

char *tdb_o_path () {
  return path_cat(sys_user_home(), ".dmCApp", "kut", "tmp.o", NULL);
}

char *tdb_so_path (char *kut_path) {
  char *key = opt_get(path_canonical(str_f("%s.kut", kut_path)));
  if (!key) EXC_KUT(str_f("Search canonical name of '%s' failed", kut_path));
  char *root = path_cat(sys_user_home(), ".dmCApp", "kut", NULL);
  char *scripts_path = path_cat(root, "scripts.tb", NULL);
  // <char>
  Arr *scripts_tb = js_ra(file_read(scripts_path));
  // <char>
  Map *scripts_map = js_ro(arr_get(scripts_tb, 1));
  char *so_name = opt_get(map_get(scripts_map, key));
  if (so_name) return path_cat(root, str_f("%s.so", so_name), NULL);
  // key is not in map
  clean_scripts(root, scripts_map);
  char *n = arr_get(scripts_tb, 0);
  map_put(scripts_map, key, n);
  arr_set(scripts_tb, 0, js_wi(js_ri(n) + 1));
  arr_set(scripts_tb, 1, js_wo(scripts_map));
  file_write(scripts_path, js_wa(scripts_tb));
  return path_cat(root, str_f("%s.so", n), NULL);
}
