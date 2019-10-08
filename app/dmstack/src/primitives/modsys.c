// Copyright 18-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modsys.h"
#include <unistd.h>
#include <pwd.h>
#include "tk.h"
#include "fails.h"
#include "args.h"

static struct {
  char *home;
  char *udir;
  char *uname;
} sys = {.home = NULL, .udir = NULL, .uname = NULL};

static void init (Machine *m) {
  char *path = tk_pop_string(m);
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
  exit(tk_pop_int(m));
}

static void locale (Machine *m) {
  machine_push(m, token_new_string(0, sys_locale()));
}

static void setlocale (Machine *m) {
  sys_set_locale(tk_pop_string(m));
}

static void cmd (Machine *m) {
  char *command = tk_pop_string(m);
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
  pthread_t *th = async_thread2(
    (FPROC)threadfn, tp_new(m, prg)
  );
  arr_push(machine_stack(m), token_from_pointer(symbol_THREAD_, th));
}

static void join (Machine *m) {
  pthread_t *th = tk_pop_pointer(m, symbol_THREAD_);
  async_join(th);
}

static void ssleep (Machine *m) {
  sys_sleep(tk_pop_int(m));
}

static void print (Machine *m) {
  fputs(tk_pop_string(m), stdout);
}

static void println (Machine *m) {
  puts(tk_pop_string(m));
}

static void error (Machine *m) {
  fputs(tk_pop_string(m), stderr);
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
  char *mark = tk_pop_string(m);
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
  char *msg = tk_pop_string(m);
  char *r = getpass(msg);
  machine_push(m, token_new_string(0, str_new(r)));
  free(r);
  getc(stdin);
}

Pmodule *modsys_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("init", init); // STRING - []
  add("init?", isinit); // [] - INT
  add("home", home); // [] - STRING
  add("uname", uname); // [] - STRING
  add("udir", udir); // [] - STRING
  add("args", args); // [] - LIST
  add("exit", sexit); // INT - []

  add("locale", locale); // STRING - STRING
  add("setLocale", setlocale); // STRING - BLOB

  add("cmd", cmd); // STRING - OPT<STRING> (command - Option<response>)

  add("freeThread", freethread); // LIST - []  (program - [])
  add("thread", thread); // LIST - LIST  (program - threadId)
  add("join", join); // LIST- [] (threadId - [])
  add("sleep", ssleep); // INT - []

  add("print", print); // STRING - []
  add("println", println); // STRING - []
  add("error", error); // STRING - []
  add("getLine", sgetline); // [] - STRING (read until enter)
  add("getText", sgettext); // STRING - STRING (read until text)
  add("getPass", sgetpass); // STRING - STRING

  return r;
}



