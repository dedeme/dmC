// Copyright 18-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modsys.h"
#include <unistd.h>
#include <pwd.h>
#include "Machine.h"
#include "args.h"

static struct {
  char *home;
  char *udir;
  char *uname;
} sys = {.home = NULL, .udir = NULL, .uname = NULL};

static void init (Machine *m) {
  char *path = token_string(machine_pop_exc(m, token_STRING));
  uid_t uid = getuid();
  struct passwd *udata = getpwuid(uid);

  if (sys.home)
    machine_fail(m, "sys init already has been called");

  sys.home = path_cat(udata->pw_dir, ".dmStack/apps", path, NULL);
  sys.uname = str_new(udata->pw_name);
  sys.udir = str_new(udata->pw_dir);
  file_mkdir(sys.home);
}

static void isinit (Machine *m) {
  machine_push(m, token_new_int(0, sys.home != 0));
}

static void home (Machine *m) {
  if (sys.home) machine_push(m, token_new_string(0, sys.home));
  else machine_fail(m, "sys init was not called");
}

static void uname (Machine *m) {
  if (sys.uname) machine_push(m, token_new_string(0, sys.uname));
  else machine_fail(m, "sys init was not called");
}

static void udir (Machine *m) {
  if (sys.udir) machine_push(m, token_new_string(0, sys.udir));
  else machine_fail(m, "sys init was not called");
}

static void args (Machine *m) {
  // Arr<Token>
  Arr *a = arr_new();
  EACH(args_dms_params(), char, p) {
    arr_push(a, token_new_string(0, p));
  }_EACH
  machine_push(m, token_new_list(0, a));
}

static void sexit (Machine *m) {
  exit(token_int(machine_pop_exc(m, token_INT)));
}

static void locale (Machine *m) {
  machine_push(m, token_new_string(0, sys_locale()));
}

static void setlocale (Machine *m) {
  sys_set_locale(token_string(machine_pop_exc(m, token_STRING)));
}

static void cmd (Machine *m) {
  char *command = token_string(machine_pop_exc(m, token_STRING));
  char *r = opt_nget(sys_cmd(command));
  if (r)
    machine_push(
      m, token_new_list(0, arr_new_c(1, (void *[]){token_new_string(0, r)}))
    );
  else
    machine_push(m, token_new_list(0, arr_new()));
}

// tp is Tp<List<Machine>, Token>
static void threadfn (Tp *tp) {
  Machine *m = tp_e1(tp);
  machine_isolate_process("", machine_pmachines(m), tp_e2(tp));
}

static void freethread (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);
  async_thread_detached(
    (FPROC)threadfn, tp_new(m, prg)
  );
}

static void thread (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);
  char *th = (char *)async_thread2(
    (FPROC)threadfn, tp_new(m, prg)
  );
  // Arr<Token>
  Arr *a = arr_new();
  arr_push(a, token_new_symbol(0, symbol_new(".thread_to_join.")));
  arr_push(a, token_new_string(0, th));
  arr_push(machine_stack(m), token_new_list(0, a));
}

static void join (Machine *m) {
  Token *thread = machine_pop_exc(m, token_LIST);
  // Arr<Token>
  Arr *a = token_list(thread);
  if (
    arr_size(a) != 2 ||
    (token_type(arr_get(a, 0)) != token_SYMBOL &&
      !str_eq(symbol_name(token_symbol(arr_get(a, 0))), ".thread_to_join.")
    )
  )
    machine_fail(m, "Expected a thread value");
  async_join((pthread_t *)token_string(arr_get(a, 1)));
}

static void ssleep (Machine *m) {
  Token *tk = machine_pop_exc(m, token_INT);
  sys_sleep(token_int(tk));
}

static void print (Machine *m) {
  Token *tk = machine_pop_exc(m, token_STRING);
  fputs(token_string(tk), stdout);
}

static void println (Machine *m) {
  Token *tk = machine_pop_exc(m, token_STRING);
  puts(token_string(tk));
}

static void error (Machine *m) {
  Token *tk = machine_pop_exc(m, token_STRING);
  fputs(token_string(tk), stderr);
}

static void sgetline (Machine *m) {
  Buf *bf = buf_new();
  char ch;
  for (;;) {
    ch = getc(stdin);
    if (ch == '\n') break;
    buf_cadd(bf, ch);
  }
  machine_push(m, token_new_string(0, buf_to_str(bf)));
}

static void sgettext (Machine *m) {
  char *mark = token_string(machine_pop_exc(m, token_STRING));
  Buf *bf = buf_new();
  char ch;
  for (;;) {
    ch = getc(stdin);
    buf_cadd(bf, ch);
    if (str_ends(buf_str(bf), mark)) break;
  }
  machine_push(m, token_new_string(
    0, str_left(buf_to_str(bf), -strlen(mark))
  ));
}

static void sgetpass (Machine *m) {
  char *msg = token_string(machine_pop_exc(m, token_STRING));
  char *r = getpass(msg);
  machine_push(m, token_new_string(0, str_new(r)));
  free(r);
  getc(stdin);
}

// Resturns Map<primitives_Fn>
Map *modsys_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "init", init); // STRING - []
  map_put(r, "init?", isinit); // [] - INT
  map_put(r, "home", home); // [] - STRING
  map_put(r, "uname", uname); // [] - STRING
  map_put(r, "udir", udir); // [] - STRING
  map_put(r, "args", args); // [] - LIST
  map_put(r, "exit", sexit); // INT - []

  map_put(r, "locale", locale); // STRING - STRING
  map_put(r, "setLocale", setlocale); // STRING - BLOB

  map_put(r, "cmd", cmd); // STRING - OPT<STRING> (command - Option<response>)

  map_put(r, "freeThread", freethread); // LIST - []  (program - [])
  map_put(r, "thread", thread); // LIST - LIST  (program - threadId)
  map_put(r, "join", join); // LIST- [] (threadId - [])
  map_put(r, "sleep", ssleep); // INT - []

  map_put(r, "print", print); // STRING - []
  map_put(r, "println", println); // STRING - []
  map_put(r, "error", error); // STRING - []
  map_put(r, "getline", sgetline); // [] - STRING (read until enter)
  map_put(r, "gettext", sgettext); // STRING - STRING (read until text)
  map_put(r, "getpass", sgetpass); // STRING - STRING

  return r;
}



