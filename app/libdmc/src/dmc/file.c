// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <string.h>
#include <stddef.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include "dmc/file.h"
#include "dmc/path.h"
#include "dmc/exc.h"
#include "dmc/str.h"
#include "dmc/DEFS.h"

char *file_cwd () {
  char *d = getcwd(NULL, 0);
  if (!d)
    THROW(exc_io_t)
      exc_io("Working directory can no be find: %s"), strerror(errno)
    _THROW

  return d;
}

void file_mkdir (char *path) {
  if (!*path) {
    return;
  } else if (*path != '/') {
    path = path_cat(file_cwd(), path, NULL);
  }
  file_mkdir(path_parent(path));
  if (mkdir(path, 0755) && errno != EEXIST)
    THROW(exc_io_t)
      exc_io("Fail making directory %s: %s"), path, strerror(errno)
    _THROW
}

