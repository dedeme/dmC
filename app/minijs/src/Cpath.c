// Copyright 18-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Cpath.h"
#include "global.h"

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

Cpath *cpath_new(char *path) {
  if (*path == '/')
    THROW "'%s' is an absolute path", path _THROW
  if (path[strlen(path) - 1] == '/')
    THROW "'%s' ends with '/'", path _THROW
  if (str_index(path, "__") != -1)
    THROW "'%s' has '//'", path _THROW


  Cpath *this = _cpath_new(path);
  char *fpath = str_printf("%s.mini", path);

  Arr/*char*/ *roots = arr_new();
  EACH(global_roots(), char, p) {
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
    THROW "'%s' not found", path _THROW

  if (arr_size(roots) > 1) {
    Buf *bf = buf_new();
    EACH(roots, char, r) {
      buf_add(bf, r);
      buf_cadd(bf, '\n');
    }_EACH
    THROW "'%s' is duplicate in:\n%s", path, buf_str(bf) _THROW
  }

  return this;
}

inline
bool cpath_eq(Cpath *this, Cpath *other) {
  return !strcmp(this->id, other->id);
}

inline
Json *cpath_serialize(Cpath *this) {
  return json_wstring(this->path);
}

inline
Cpath *cpath_restore(Json *s) {
  return cpath_new(json_rstring(s));
}

