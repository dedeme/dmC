// Copyright 31-May-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "db/log.h"
#include "kut/file.h"
#include "kut/path.h"
#include "kut/arr.h"
#include "kut/str.h"
#include "kut/js.h"
#include "data/cts.h"

static char *path (void) {
  return path_cat(DATA_PATH, "log.tb", NULL);
}

static char *mkFieldJs (int isError, char *msg) {
  return js_wa(arr_new_from(
    js_wb(isError), js_ws(time_f(time_now(), "%D/%M/%Y(%T)")), js_ws(msg), NULL
  ));
}

void log_init () {
  char *p = path();
  if (!file_exists(p)) file_write(p, "[]");
}

char *log_msg(char *message, char *file, char *func, int line) {
  return str_f(
    "%s:%d:[%s]: %s", path_base(file), line, func, message
  );
}

/// Write an error message.
void log_error (char *msg) {
  char *p = path();
  // <char>
  Arr *l = js_ra(file_read(p));
  arr_push(l, mkFieldJs(1, msg));
  file_write(p, js_wa(arr_drop(l, arr_size(l) - 1000)));
}

/// Write an error warning.
void log_warning (char *msg) {
  char *p = path();
  // <char>
  Arr *l = js_ra(file_read(p));
  arr_push(l, mkFieldJs(0, msg));
  file_write(p, js_wa(arr_drop(l, arr_size(l) - 1000)));
}

/// Reset log.
void log_reset () {
  file_write(path(), "[]");
}

/// Return log content in JSON format.
/// Log content is an array with each entry equals to:
///   isError: boolean. 'true' if the entri is an error.
///   date: char*. Date in format "%D/%M/%Y(%T)"
///   msg: char*. Entry message.
char *log_readJs (void) {
  return file_read(path());
}

