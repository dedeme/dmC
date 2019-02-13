// Copyright 11-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "io.h"
#include "dmc/date.h"
#include "DEFS.h"
#include "Server.h"

void io_init (void) {
  char *home = sys_home();
  char *path = path_cat_new(home, "state.db", NULL);
  if (file_exists(path)) {
    free(path);
    return;
  }
  free(path);
  file_mkdir(home);
  io_state_write(ST_SLEEPING);
  // Arr[char]
  Arr *svs = arr_new(free);
  io_servers_write(svs);
  arr_free(svs);
  path = path_cat_new(home, "log.db", NULL);
  file_write(path, "[]");
  free(path);
  path = path_cat_new(sys_home(), "servers", NULL);
  file_mkdir(path);
  free(path);
  path = path_cat_new(sys_home(), "nicks", NULL);
  file_mkdir(path);
  free(path);
}

void io_lock_write (char *key) {
  char *path = path_cat_new(sys_home(), "lock", NULL);
  file_write(path, key);
  free(path);
}

void io_lock_del (void) {
  char *path = path_cat_new(sys_home(), "lock", NULL);
  file_del(path);
  free(path);
}

int io_lock_check (char *key) {
  char *path = path_cat_new(sys_home(), "lock", NULL);
  if (file_exists(path)) {
    char *k = file_read_new(path);
    int r = str_eq(k, key);
    free(k);
    free(path);
    return r;
  }
  free(path);
  return 0;
}

static void io_log (char *msg, int is_error) {
  char *date = date_f_new(date_now(), "%d/%m/%Y|%H:%M:%S");
  char *m = is_error
    ? str_f_new("e%s|%s", date, msg)
    : str_f_new("w%s|%s", date, msg)
  ;
  char *path = path_cat_new(sys_home(), "log.db", NULL);
  char *js = file_read_new(path);
  // Arr[char]
  Arr *ms = arr_from_js_new((Js *)js, (FFROM)js_rs_new, free);
  arr_push(ms, m);
  while (arr_size(ms) > 100) {
    arr_remove(ms, 0);
  }
  free(js);
  js = (char *)arr_to_js_new(ms, (FTO)js_ws_new);
  file_write(path, js);
  free(js);
  arr_free(ms);
  free(path);
  free(date);
}

void io_loge (const char *format, ...) {
  va_list args;
  va_start(args, format);
  char *msg = str_vf_new(format, args);
  va_end(args);
  io_log(msg, 1);
  free(msg);
}

void io_logw (const char *format, ...) {
  va_list args;
  va_start(args, format);
  char *msg = str_vf_new(format, args);
  va_end(args);
  io_log(msg, 0);
  free(msg);
}

void io_state_write (char *state) {
  char *path = path_cat_new(sys_home(), "state.db", NULL);
  file_write(path, state);
  free(path);
}

void io_state_read (char **state_new) {
  char *path = path_cat_new(sys_home(), "state.db", NULL);
  *state_new = file_read_new(path);
  free(path);
}

// box is Arr[char]
void io_servers_write (Arr *box) {
  char *path = path_cat_new(sys_home(), "servers.db", NULL);
  Js *js = arr_to_js_new(box, (FTO) js_ws_new);
  file_write(path, (char *)js);
  free(js);
  free(path);
}

// io_servers_read_new is Arr[char]
Arr *io_servers_read_new (void) {
  char *path = path_cat_new(sys_home(), "servers.db", NULL);
  Js *js = (Js *)file_read_new(path);
  // Arr[char]
  Arr *r = arr_from_js_new(js, (FFROM) js_rs_new, free);
  free(js);
  free(path);
  return r;
}

void io_server_current_write(char *server_name) {
  char *path = path_cat_new(sys_home(), "current_server.db", NULL);
  file_write(path, server_name);
  free(path);
}

/// Reads the current server name
char *io_server_current_read_new(void) {
  char *path = path_cat_new(sys_home(), "current_server.db", NULL);
  char *r;
  if (file_exists(path)) {
    r = file_read_new(path);
  } else {
    r = str_new("");
  }
  free(path);
  return r;
}

void io_server_write(char *id, char *js) {
  char *path = path_cat_new(sys_home(), "servers", id, NULL);
  file_write(path, js);
  free(path);
}

char *io_server_read_new(char *id) {
  char *path = path_cat_new(sys_home(), "servers", id, NULL);
  if (!file_exists(path)) {
    free(path);
    return str_new("");
  }
  char *r = file_read_new(path);
  free(path);
  return r;
}

void io_nick_write(char *nick, char *data) {
  char *path = path_cat_new(sys_home(), "nicks", nick, NULL);
  file_write(path, data);
  free(path);
}

char *io_nick_read_new(char *nick) {
  char *path = path_cat_new(sys_home(), "nicks", nick, NULL);
  char *r = file_read_new(path);
  free(path);
  return r;
}

Arr *io_nicks_new(void) {
  char *path = path_cat_new(QUOTES, "nicks.db", NULL);
  Js *js = (Js *)file_read_new(path);
  free(path);

  // Arr[Js]
  Arr *data = js_ra_new(js);

  // Arr[Js]
  Arr *nicks = js_ra_new(arr_get(data, 2));

  // Arr[char]
  Arr *r = arr_new(free);
  EACH(nicks, Js, js)
    //Arr[Js]
    Arr *nk = js_ra_new(js);
    if (js_rb(arr_get(nk, 3))) {
      arr_push(r, js_rs_new(arr_get(nk, 1)));
    }
    arr_free(nk);
  _EACH

  arr_free(nicks);
  arr_free(data);
  free(js);

  return r;
}
