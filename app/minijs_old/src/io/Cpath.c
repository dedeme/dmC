// Copyright 03-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <gc.h>
#include "dmc/exc.h"
#include "dmc/str.h"
#include "dmc/DEFS.h"
#include "dmc/ct/Achar.h"
#include "dmc/ct/Ajson.h"
#include "dmc/file.h"
#include "dmc/path.h"
#include "dmc/sys.h"
#include "dmc/Buf.h"
#include "dmc/Json.h"
#include "lexer/Tx.h"
#include "io/Cpath.h"
#include "ct/Acpath.h"
#include "DEFS.h"

static Achar *_roots = NULL;

/*.
-struct: @Cpath
  path: char *
  -
  fpath: char * = ""
  id: char * = ""
  file: char * = ""
  parent: char * = ""
  lib: char * = ""
  js: char * = ""
*/

/*.-.*/
struct cpath_Cpath {
  char *path;

  char *fpath;
  char *id;
  char *file;
  char *parent;
  char *lib;
  char *js;
};

Cpath *_cpath_new(char *path) {
  Cpath *this = MALLOC(Cpath);
  this->path = path;
  this->fpath = "";
  this->id = "";
  this->file = "";
  this->parent = "";
  this->lib = "";
  this->js = "";
  return this;
}

inline
char *cpath_path(Cpath *this) {
  return this->path;
}

inline
char *cpath_fpath(Cpath *this) {
  return this->fpath;
}

inline
char *cpath_id(Cpath *this) {
  return this->id;
}

inline
char *cpath_file(Cpath *this) {
  return this->file;
}

inline
char *cpath_parent(Cpath *this) {
  return this->parent;
}

inline
char *cpath_lib(Cpath *this) {
  return this->lib;
}

inline
char *cpath_js(Cpath *this) {
  return this->js;
}
/*.-.*/

inline
void cpath_init(Achar *roots) {
  _roots = roots;
}

Cpath *cpath_new(Tx *tx, char *path) {
  XNULL(tx)
  XNULL(path)
  if (*path == '/')
    TH(tx) "'%s' is an absolute path", path _TH
  if (path[str_len(path) - 1] == '/')
    TH(tx) "'%s' ends with '/'", path _TH
  if (str_index(path, "__") != -1)
    TH(tx) "'%s' has '//'", path _TH
  if (str_index(path_name(path), "_") != -1)
    TH(tx) "File name of '%s' has '_'", path _TH


  Cpath *this = _cpath_new(path);
  char *fpath = str_printf("%s.mini", path);

  Achar *roots = achar_new();
  EACH((Arr *)_roots, char, p) {
    if (file_exists(path_cat(p, fpath, NULL))) {
      achar_add(roots, p);
      this->id = str_creplace(path, '/', '_');
      char *f = *p == '/'
        ? path_cat(p, path, NULL)
        : path_cat(file_cwd(), p, path, NULL);
      this->fpath = fpath;
      this->file = str_printf("%s.mini", f);
      this->parent = path_parent(this->file);
      this->lib = path_cat(sys_home(), "paths", f + 1, NULL);
      this->js = str_printf("%s.js", this->lib);
    }
  }_EACH

  if (!achar_size(roots))
    TH(tx) "'%s' not found", path _TH

  if (achar_size(roots) > 1) {
    Buf *bf = buf_new();
    EACH((Arr *)roots, char, r) {
      buf_add(bf, r);
      buf_cadd(bf, '\n');
    }_EACH
    TH(tx) "'%s' is duplicate in:\n%s", path, buf_str(bf) _TH
  }

  return this;
}

Cpath *cpath_from_id(Tx *tx, char *id) {
  XNULL(tx)
  XNULL(id)
  return cpath_new(tx, str_creplace(id, '_', '/'));
}

bool cpath_eq(Cpath *this, Cpath *other) {
  XNULL(this)
  XNULL(other)
  return str_eq(this->id, other->id);
}

Ajson *cpath_serialize(Cpath *this) {
  XNULL(this)
  Ajson *r = ajson_new();
  ajson_add(r, json_wstring(this->path));
  return r;
}

Cpath *cpath_restore(Ajson *s) {
  XNULL(s)
  char *path = json_rstring(ajson_get(s, 0));
  char *tmp = "";
  return cpath_new(tx_new("RESTORE", tmp, tmp, 0, 0), path);
}

char *cpath_absolute(char *dir) {
  XNULL(dir)

  if (
    *dir == '/' ||
    dir[str_len(dir) - 1] == '/'
  ) {
    return "";
  }

  EACH((Arr *)_roots, char, p) {
    char *r = *p == '/'
      ? path_cat(p, dir, NULL)
      : path_cat(file_cwd(), p, dir, NULL);
    if (file_exists(r)) {
      return r;
    }
  }_EACH

  return "";
}
