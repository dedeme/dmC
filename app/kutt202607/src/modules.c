// Copyright 16-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "modules.h"
#include "kut/file.h"
#include "data/mdRc.h"
#include "readers/moduleRd.h"
#include "db.h"

static Arr *mds = NULL;

void modules_init (void) {
  mds = arr_new();
}

Module *modules_get (char *id) {
  EACH(mds, Module, m) {
    if (str_eq(m->id, id)) return m;
  }_EACH
  EXC_KUTT(str_f("Module '%s'not found.", id));
  return NULL; // Unreachable
}

void modules_each (void (*fn)(Module *md)) {
  EACH(mds, Module, m) {
    fn(m);
  }_EACH
}

char *modules_add(int is_main, char *path, char *id) {
  EACH(mds, Module, m) {
    if (str_eq(m->id, id)) return ""; // Module read
  }_EACH

  // Push a fake module to prevent recompilations.
  int ix = arr_size(mds);
  arr_push(mds, module_new(
    is_main, path, id, map_new(), arr_new(), arr_new(), map_new(), arr_new()
  ));

  Rs *rs = moduleRd_read(is_main, path, id, file_read(path));

  Module *md = rs_get(rs);
  if (!md) return rs_error(rs); // Error
  arr_begin(mds)[ix] = md;
  db_write_module(
    mdRc_new(
      path,
      id,
      md->imports,
      md->exports,
      md->type_defs
    ), id
  );
  return "";
}
