// Copyright 22-Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <dlfcn.h>
#include "DEFS.h"
#include "kut/path.h"
#include "kut/file.h"
#include "kut/buf.h"
#include "kut/sys.h"
#include "kut/rs.h"
#include "typed/genc.h"
#include "typed/builtIn.h"
#include "typed/treader.h"
#include "typed/tdb.h"
#include "fileix.h"
#include "symix.h"

static int without_tfunctions = TRUE;
static char *kut_path = "";
static Buf *head = NULL;
static Buf *body = NULL;
void *lib_handle = NULL;

void genc_init (char *path) {
  tdb_init(path);
  if (str_cindex(path, '/') == -1) path = str_f("./%s", path);
  kut_path = path;
  head = buf_new();
  body = buf_new();
}

void genc_write (Tfunction *fn) {
  without_tfunctions = FALSE;
  char *fn_id = tfunction_get_cid(fn);


  Ttype tret = tfunction_get_tret(fn);
  if (tret == ttype_NO_RETURN) {
    buf_add(head, str_f("void %s(Tval *_values);\n", fn_id));
    buf_add(body, str_f("void %s(Tval *_values){\n", fn_id));
  } else {
    buf_add(head, str_f("Tval %s(Tval *_values);\n", fn_id));
    buf_add(body, str_f("Tval %s(Tval *_values){\n", fn_id));
  }

  Iarr *pars = tfunction_get_pars(fn);
  Iarr *tpars = tfunction_get_tpars(fn);
  RANGE0(i, iarr_size(pars)) {
    char *sym = symix_get(iarr_get(pars, i));
    switch ((Ttype)iarr_get(tpars, i)) {
      case ttype_BOOL:
        buf_add(body, str_f("int %s = _values[%d].b;\n", sym, i));
        break;
      case ttype_INT:
        buf_add(body, str_f("int64_t %s = _values[%d].i;\n", sym, i));
        break;
      case ttype_FLOAT:
        buf_add(body, str_f("double %s = _values[%d].f;\n", sym, i));
        break;
      case ttype_STRING:
        buf_add(body, str_f("char *%s = _values[%d].s;\n", sym, i));
        break;
      case ttype_AINT:
        buf_add(body, str_f("Arri *%s = _values[%d].I;\n", sym, i));
        break;
      case ttype_AFLOAT:
        buf_add(body, str_f("Arrf *%s = _values[%d].F;\n", sym, i));
        break;
      default:
        buf_add(body, str_f("Arrs *%s = _values[%d].S;\n", sym, i));
        break;
    }
  }_RANGE

  Theap *th = theap_new();
  StatCode *st_cd = tfunction_get_st_cd(fn);
  RANGE0(i, iarr_size(tpars)) {
    theap_add(th, iarr_get(pars, i), iarr_get(tpars, i), st_cd);
  }_RANGE
  treader_read(body, tret, th, st_cd);

  buf_add(body, "}\n\n");
}

void genc_compile (void) {
  if (without_tfunctions) return;

  char *so_path = tdb_so_path(kut_path, TRUE);
  char *c_path = tdb_c_path();
  char *o_path = tdb_o_path();

  char *h = str_f("%s%s", builtIn_head, buf_str(head));
  char *b = str_f("%s%s", builtIn_body, buf_str(body));

  char *code = str_f("%s\n// -----\n// -----\n\n%s", h, b);
  if (strcmp(code, file_read(c_path))) {
    file_write(c_path, code);

    Rs *rs1 = sys_cmd(str_f(
      "gcc -Wno-div-by-zero -c -rdynamic -fPIC %s -o %s -lm -lgc",
      c_path, o_path
    ));
    if (!rs_get(rs1)) puts(rs_error(rs1));
    Rs *rs2 = sys_cmd(str_f("gcc -shared %s -o %s", o_path, so_path));
    if (!rs_get(rs2)) puts(rs_error(rs2));

    file_del(o_path);
  }
}

void exc_fns (char type, void *data) {
  switch (type) {
    case '-': exc_remove(); break;
    case '+': exc_add(data); break;
    case '<': *(char **)data = exc_msg(exc_get()); break;
    default: EXC_KUT(data); // type == ':'
  }
}
void fail_fn (char *msg) {
  EXC_KUT(msg);
}
void genc_load_so (void) {
  char *so_path = tdb_so_path(kut_path, FALSE);
  if (file_exists(so_path)) {
    lib_handle = dlopen(so_path, RTLD_LAZY);
    if (!lib_handle) EXC_KUT(dlerror());
    dlerror();    // Clear dl-error

    void (*set_exc)(void (*fn)(char, void *));
    *(void **) (&set_exc) = dlsym(lib_handle, "built_set_exc");
    char *error = "";
    if ((error = dlerror()) != NULL) EXC_KUT(error);
    dlerror();  // Clear dl-error
    set_exc(exc_fns);
  }
}

void genc_run_procedure (char *id, Tval *values) {
  void (*fn) (Tval *);
  *(void **) (&fn) = dlsym(lib_handle, id);
  char *error = "";
  if ((error = dlerror()) != NULL)
    EXC_KUT(str_f("%s\n  Try to compile with 'kut -c'", error));
  dlerror();  // Clear dl-error
  fn(values);
}

/// Execute C function with return.
Tval genc_run_function (char *id, Tval *values) {
  Tval (*fn) (Tval *);
  *(void **) (&fn) = dlsym(lib_handle, id);
  char *error = "";
  if ((error = dlerror()) != NULL)
    EXC_KUT(str_f("%s\n  Try to compile with 'kut -c'", error));
  dlerror();  // Clear dl-error
  return fn(values);
}
