// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities to manage paths and class names<br>
/// <h3>Returns notes:</h3>
/// <ul>
/// <li><tt>cpath_fpath</tt>: returns "d1/File.mini"
/// <li><tt>cpath_id...</tt>: returns "d1_File"
/// <li><tt>cpath_file.</tt>: returns "/home/me/d1/file.mini"
/// <li><tt>cpath_lib..</tt>: returns "(sys_home())/paths/d1/file.mini"
/// </ul>

#ifndef CPATH_H
  #define CPATH_H

#include <dmc/all.h>

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
char *cpath_lib(Cpath *this);

/*.-.*/

/// Must be called only once and before cpath_new()
void cpath_init(Arr/*char*/ *paths);

/// Throws an exception if the corresponding file path is duplicate or does
/// not exist. Also throws an expception if 'path' is absolute.
Cpath *cpath_new(char *path);

///
Json *cpath_serialize(Cpath *this);

///
Cpath *cpath_restore(Json *s);

#endif
