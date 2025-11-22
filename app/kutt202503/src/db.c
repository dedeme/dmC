// Copyright 10-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "db.h"
#include "cts.h"
#include "kut/file.h"
#include "kut/path.h"
#include "kut/rs.h"
#include "kut/str.h"
#include "kut/sys.h"
#include "kut/cryp.h"
#include "c/cbody.h"
#include "c/cheader.h"
#include "c/interface.h"
#include "data/index.h"
#include "data/mdRc.h"

static char *root = NULL;

//  Returns the module index [[canonicalPath, module].]}
static Index *read_module_index (void) {
  return index_from_js(file_read(
    path_cat(cts_data_path(), "index.tb", NULL)
  ));
}

//  Writes the JSONized module index [[canonicalPath, module].]}
static void write_module_index (Index *index) {
  file_write(
    path_cat(cts_data_path(), "index.tb", NULL),
    index_to_js(index)
  );
}

IndexEntry *db_init (char *fmain) {
  char *dbroot = cts_data_path();
  char *fversion = path_cat(dbroot, "version.txt", NULL);
  if (
    !file_exists(dbroot) || !file_exists(fversion) ||
    strcmp(str_trim(file_read(fversion)), cts_version)
  ) {
    file_mkdir(cts_compilation_path());
    file_write(fversion, cts_version);
    file_write(cts_h_built_path(), cheader_code());
    file_write(cts_c_built_path(), cbody_code());
    Rs *rs = sys_cmd(arr_new_from(
      "gcc ",
        "-Wall", "-Wno-div-by-zero", "-c", "-rdynamic",
        "-z", "execstack",
        cts_c_built_path(),
        "-o",  cts_o_built_path(),
        "-lgc", "-lm", "-lpthread", NULL
    ));
    if (*rs_error(rs)) EXC_KUTT(rs_error(rs));
    Rs *rs2 = sys_cmd(arr_new_from(
      "ar", "rcs", cts_a_built_path(), cts_o_built_path(), NULL
    ));
    if (*rs_error(rs2)) EXC_KUTT(rs_error(rs2));
    file_del(cts_o_built_path());
    file_del(cts_c_built_path());

    write_module_index(arr_new());
  }
  IndexEntry *r = db_get_path_id(str_f("%s.ktt", fmain));
  if (*r->path) root = path_parent(r->path);
  return r;
};

char *db_main_root (void) {
  return root;
}

IndexEntry *db_get_path_id (char *path) {
  if (!file_exists(path)) return indexEntry_new("", ""); // <-- Return error.
  char *cpath = opt_eget(path_canonical(path));
  Index *list = read_module_index();
  EACH(list, IndexEntry, e) {
    if (str_eq(e->path, cpath)) return e;  // <-- Return id existent
  }_EACH
  char *base = str_left(path_base(path), -4);
  IndexEntry *r = NULL;
  for (;;) {
    char *id = str_replace(
      str_replace(str_f("%s_%s", base, cryp_genk(4)), "/", "0"),
      "+", "1"
    );
    int stop = TRUE;
    EACH(list, IndexEntry, e) {
      if (str_eq(e->id, id)) {
        stop = FALSE;
        break;
      }
    }_EACH
    if (stop) {
      r = indexEntry_new(cpath, id);
      arr_push(list, r);
      write_module_index(list);
      break;
    }
  }
  return r;
}

Opt *db_read_module (char *id) {
  char *path = path_cat(cts_compilation_path(), str_f("%s.mod", id), NULL);
  return file_exists(path)
    ? opt_some(mdRc_from_js(file_read(path)))
    : opt_none()
  ;
}

void db_write_module (MdRc *mod, char *id) {
  char *path = path_cat(cts_compilation_path(), str_f("%s.mod", id), NULL);
  file_write(path, mdRc_to_js(mod));
}

int64_t db_module_date (char *mod) {
  char *path = path_cat(cts_compilation_path(), str_f("%s.mod", mod), NULL);
  if (file_exists(path)) return file_modified(path);
  return -1;
}

int64_t db_object_date (char *mod) {
  char *path = path_cat(cts_compilation_path(), str_f("%s.o", mod), NULL);
  if (file_exists(path)) return file_modified(path);
  return -1;
}

void db_clean (void) {
  char *cp = cts_compilation_path();
  if (!file_exists(cp)) return;
  Index *index = read_module_index();
  Arr *ids = arr_new(); // Arr<char>
  Index *newIndex = arr_new();
  EACH(index, IndexEntry, e) {
    if (file_exists(e->path)){
      arr_push(ids, e->id);
      arr_push(newIndex, e);
    }
  }_EACH
  write_module_index(index);
  EACH(file_dir(cp), char, fname) {
    char *path = path_cat(cp, fname, NULL);
    char *ex = path_extension(fname);
    int ex_len = strlen(ex);
    int findex (char *e) { return str_eq(e, ex); }
    if (arr_index(
      arr_new_from(".c", ".mod", ".bin", ".o", NULL),
      (FPRED)findex
    )){
      char *name = str_left(fname, -ex_len);
      int fany (char *e) { return str_eq(e, name); }
      if (arr_any(ids, (FPRED)fany)) continue;
    }
    file_del(path);
  }_EACH
};

void db_delete (void) {
  char *cp = cts_compilation_path();
  if (!file_exists(cp)) return;
  file_del(cp);
  file_mkdir(cp);
};

