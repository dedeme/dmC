// Copyright 31-May-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "db.h"
#include "kut/DEFS.h"
#include "kut/file.h"
#include "kut/path.h"
#include "kut/sys.h"
#include "data/cts.h"
#include "db/log.h"
#include <stdio.h>

void db_init (void) {
  char *root = cts_data_path();
  if (!file_exists(root)) file_mkdir(root);

  log_init();
}

int db_lock (void) {
  char *p = path_cat(cts_data_path(), "lock", NULL);
  if (file_exists(p)) {
    Time time_out = time_now() + 10000;
    for (;;) {
      if (time_now() > time_out) return 0;
      if (!file_exists(p)) break;
      sys_sleep(500);
    }
  }
  file_write(p, "");
  return 1;
}

void db_unlock (void) {
  file_del(path_cat(cts_data_path(), "lock", NULL));
}

/// Read a table.
char *db_read (char *path) {
  char *p = path_cat(cts_data_path(), "lock", NULL);
  if (file_exists(p)) {
    Time time_out = time_now() + 10000;
    for (;;) {
      if (time_now() > time_out)
        EXC_ILLEGAL_STATE(str_f("Date base locked reading '%s'", path));
      if (!file_exists(p)) break;
      sys_sleep(500);
    }
  }
  return file_read(p);
}

/// Write text in table
void db_write (char *path, char *text) {
  if (!db_lock())
    EXC_ILLEGAL_STATE(str_f("Date base locked writing '%s'", path));

  file_write(path, text);
  db_unlock();
}
