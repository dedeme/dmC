// Copyright 18-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Cpath.h"
#include "lexer/Tx.h"
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

Cpath *cpath_new(char *path) {
  if (!_roots)
    THROW exc_null_pointer("_roots") _THROW

  Pos *pos = pos_new(0, 0);
  if (*path == '/')
    TH1(path, pos) "'%s' is an absolute path", path _TH
  if (path[strlen(path) - 1] == '/')
    TH1(path, pos) "'%s' ends with '/'", path _TH
  if (str_index(path, "__") != -1)
    TH1(path, pos) "'%s' has '//'", path _TH


  Cpath *this = _cpath_new(path);
  char *fpath = str_printf("%s.mini", path);

  Arr/*char*/ *roots = arr_new();
  EACH(_roots, char, p) {
    if (file_exists(path_cat(p, fpath, NULL))) {
      arr_add(roots, p);
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

  if (!arr_size(roots))
    TH1(path, pos) "'%s' not found", path _TH

  if (arr_size(roots) > 1) {
    Buf *bf = buf_new();
    EACH(roots, char, r) {
      buf_add(bf, r);
      buf_cadd(bf, '\n');
    }_EACH
    TH1(path, pos) "'%s' is duplicate in:\n%s", path, buf_str(bf) _TH
  }

  return this;
}

inline
Cpath *cpath_from_id(char *id) {
  return cpath_new(str_creplace(id, '_', '/'));
}

inline
bool cpath_eq(Cpath *this, Cpath *other) {
  return !strcmp(this->id, other->id);
}

inline
Arr/*Json*/ *cpath_serialize(Cpath *this) {
  Arr/*Json*/ *r = arr_new();
  arr_add(r, json_wstring(this->path));
  return r;
}

inline
Cpath *cpath_restore(Arr/*Json*/ *s) {
  return cpath_new(json_rstring(arr_get(s, 0)));
}

Arr/*Cpath*/ *cpath_dirs(char *dir) {
  if (!_roots)
    THROW exc_null_pointer("_roots") _THROW

  Achar *dirs = _roots;
  if (*dir) {
    dirs = achar_new();
    EACH(_roots, char, r) {
      char *d = path_cat(r, dir, NULL);
      if (file_is_directory(d)) {
        achar_add(dirs, d);
      }
    }_EACH
  }

  Arr/*Cpath*/ *r = arr_new();
  EACH(dirs, char, d) {
    EACH(file_dir(d), char, f) {
      if (str_ends(f, ".mini")) {
        arr_add(r, cpath_new(str_printf(
          *dir ? "%s/%s" : "%s%s", dir, path_only_name(f)
        )));
      }
    }_EACH
  }_EACH

  return r;
}
