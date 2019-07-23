// Copyright 17-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "log.h"

void log_error (char *msg) {
  puts(log_error_s(msg));
}

char *log_error_s (char *msg) {
  /// Map[Js]
  Map *rp = map_new();
  map_put(rp, "error", js_ws(msg));
  return (char *)js_wo(rp);
}

void log_ok (Js *value) {
  puts(log_ok_s(value));
}

char *log_ok_s (Js *value) {
  /// Map[Js]
  Map *rp = map_new();
  map_put(rp, "error", js_ws(""));
  map_put(rp, "value", value);
  return (char *)js_wo(rp);
}
