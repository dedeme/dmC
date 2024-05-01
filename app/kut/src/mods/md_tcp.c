// Copyright 08-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "kut/thread.h"
#include "mods/md_tcp.h"
#include "kut/tcp.h"
#include "kut/dec.h"
#include "kut/time.h"
#include "kut/sys.h"
#include "exp.h"
#include "function.h"
#include "obj.h"
#include "runner/fail.h"

// \<tcpServer> -> [<tcpConn>, s]
static Exp *f_accept (Arr *exps) {
  CHECK_PARS ("tcp.accept", 1, exps);
  // <tcpConn>
  Rs *r = tcp_accept(obj_rget_tcp_server(arr_get(exps, 0)));
  if (rs_get(r))
    return exp_array(arr_new_from(obj_tcp_conn(rs_get(r)), exp_string(""), NULL));
  return exp_array(arr_new_from(exp_empty(), exp_string(rs_error(r)), NULL));

}

// \<tcpConn> -> ()
static Exp *close_connection (Arr *exps) {
  CHECK_PARS ("tcp.closeConnection", 1, exps);
  tcp_close_conn(obj_rget_tcp_conn(arr_get(exps, 0)));
  return exp_empty();
}

// \<tcpServer> -> ()
static Exp *close_server (Arr *exps) {
  CHECK_PARS ("tcp.closeServer", 1, exps);
  tcp_close_server(obj_rget_tcp_server(arr_get(exps, 0)));
  return exp_empty();
}

// \s, i -> [<tcpConn>, s]
static Exp *dial (Arr *exps) {
  CHECK_PARS ("tcp.dial", 2, exps);
  char *sv = exp_rget_string(arr_get(exps, 0));
  int ok = TRUE;
  Arr *ps = str_csplit(sv, ':');
  if (arr_size(ps) != 2) ok = FALSE;
  if (ok && !dec_digits(arr_get(ps, 1))) ok = FALSE;
  if (!ok)
    return exp_array(arr_new_from(
      exp_empty(),
      exp_string(str_f("Bad sever:port (%s)", sv)),
      NULL
    ));

  Rs *r = tcp_dial(arr_get(ps, 0), dec_stoi(arr_get(ps, 1)));
  if (rs_get(r))
    return exp_array(arr_new_from(obj_tcp_conn(rs_get(r)), exp_string(""), NULL));
  return exp_array(arr_new_from(exp_empty(), exp_string(rs_error(r)), NULL));
}

// \<tcpConn>, i, i -> [<bytes>, s]
static Exp *f_read (Arr *exps) {
  CHECK_PARS ("tcp.read", 3, exps);
  // <bytes>
  Rs *r = tcp_read(
    obj_rget_tcp_conn(arr_get(exps, 0)),
    exp_rget_int(arr_get(exps, 1)),
    exp_rget_int(arr_get(exps, 2))
  );
  if (rs_get(r))
    return exp_array(arr_new_from(obj_bytes(rs_get(r)), exp_string(""), NULL));
  return exp_array(arr_new_from(exp_empty(), exp_string(rs_error(r)), NULL));
}

// \i -> <tcpServer>
static Exp *server (Arr *exps) {
  CHECK_PARS ("tcp.server", 2, exps);
  signal(SIGPIPE, SIG_IGN);
  return obj_tcp_server(tcp_server(
    exp_rget_int(arr_get(exps, 0)),
    exp_rget_int(arr_get(exps, 1))
  ));
}

// \<tcpConn>, <bytes> -> [s]|[]
static Exp *f_write (Arr *exps) {
  CHECK_PARS ("tcp.writeBin", 2, exps);
  return exp_string(tcp_write(
    obj_rget_tcp_conn(arr_get(exps, 0)),
    obj_rget_bytes(arr_get(exps, 1))
  ));
}

Bfunction md_tcp_get (char *fname) {
  if (!strcmp(fname, "accept")) return f_accept;
  if (!strcmp(fname, "closeConnection")) return close_connection;
  if (!strcmp(fname, "closeServer")) return close_server;
  if (!strcmp(fname, "dial")) return dial;
  if (!strcmp(fname, "read")) return f_read;
  if (!strcmp(fname, "server")) return server;
  if (!strcmp(fname, "write")) return f_write;

  EXC_KUT(fail_bfunction("tcp", fname));
  return NULL; // Unreachable
}
