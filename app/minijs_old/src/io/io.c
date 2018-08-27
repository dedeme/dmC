// Copyright 03-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>

#include <stddef.h>
#include <sys/stat.h>
#include "dmc/ct/Ochar.h"
#include "dmc/sys.h"
#include "dmc/path.h"
#include "dmc/str.h"
#include "dmc/file.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "io/Cpath.h"
#include "DEFS.h"

void io_check_install(void) {
  void throw(char *dir) {
    THROW(exc_io_t) "Bad installation. Try using %s.", INSTALL_SCRIPT _THROW
  }

  char *path = path_cat(sys_home(), BUILT_DIR, NULL);
  if (!file_exists(path)) {
    throw(path);
  }
  path = path_cat(sys_home(), PATHS_DIR, NULL);
  if (!file_exists(path)) {
    throw(path);
  }
}

Ochar *io_rclass_serial(Cpath *path) {
  char *flib = cpath_lib(path);
  if (!file_exists(flib)) {
    return ochar_null();
  }

  struct stat *st = file_info(flib);
  time_t tlib = st->st_mtime;

  char *fdisk = cpath_file(path);
  st = file_info(fdisk);
  time_t tdisk = st->st_mtime;

  if (tdisk > tlib) {
    return ochar_null();
  }

  return ochar_new(file_read(flib));
}

char *io_rclass_code(Cpath *path) {
  return file_read(cpath_file(path));
}

void io_wclass(Cpath *path, char *serial) {
  char *fpath = cpath_lib(path);
  char *parent = path_parent(fpath);
  if (!file_exists(parent)) {
    file_mkdir(parent);
  }
  file_write(fpath, serial);
}
