// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities to manage paths and class names<br>
/// <h3>Returns notes:</h3>
/// <ul>
/// <li><tt>cpath_path..</tt>: returns "d1/File" (used in constructor)
/// <li><tt>cpath_fpath.</tt>: returns "d1/File.mini"
/// <li><tt>cpath_id....</tt>: returns "d1_File"
/// <li><tt>cpath_file..</tt>: returns "/home/me/d1/file.mini"
/// <li><tt>cpath_parent</tt>: returns "/home/me/d1"
/// <li><tt>cpath_lib...</tt>: returns "(sys_home())/paths/d1/file"
/// <li><tt>cpath_js....</tt>: returns "(sys_home())/paths/d1/file.js"
/// </ul>

#ifndef CPATH_H
  #define CPATH_H

#include <Achar.h>

/*.-.*/

///
typedef struct cpath_Cpath Cpath;

///
char *cpath_path(Cpath *this);

///
char *cpath_fpath(Cpath *this);

///
char *cpath_id(Cpath *this);

///
char *cpath_file(Cpath *this);

///
char *cpath_parent(Cpath *this);

///
char *cpath_lib(Cpath *this);

///
char *cpath_js(Cpath *this);

/*.-.*/

/// Must be called only once and before cpath_new()
void cpath_init(Achar *roots);

/// Throws an exception if the corresponding file path is duplicate or does
/// not exist. Also throws an expception if 'path' is absolute.
///    path: Does not take extension.
Cpath *cpath_new(char *path);

/// Creates a Cpath from its cpath_id. It calls cpath_new
Cpath *cpath_from_id(char *id);

///
bool cpath_eq(Cpath *this, Cpath *other);

///
Json *cpath_serialize(Cpath *this);

///
Cpath *cpath_restore(Json *s);

/// Returns every cpath in relative directory 'dir'.
Arr/*Cpath*/ *cpath_dirs(char *dir);

#endif
