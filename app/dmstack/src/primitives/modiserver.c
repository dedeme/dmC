// Copyright 23-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modiserver.h"
#include "dmc/Iserver.h"
#include "fails.h"
#include "tk.h"

static void open (Machine *m) {
  int port = tk_pop_int(m);
  machine_push(m, token_from_pointer(symbol_ISERVER_, iserver_new(port)));
}

static void close (Machine *m) {

  Iserver *s = (Iserver *)tk_pop_native(m, symbol_ISERVER_);
  iserver_close(s);
}

static void getrq (Machine *m) {
  Iserver *s = (Iserver *)tk_pop_native(m, symbol_ISERVER_);
  IserverRq *rq = iserver_read(s);
  char *err = iserverRq_error(rq);
  if (*err) {
    machine_push(m, token_new_list(arr_new_from(
      token_new_string(err), token_new_int(0), NULL
    )));
  } else {
    machine_push(m, token_new_list(arr_new_from(
      token_from_pointer(symbol_ISERVER_RQ_, rq),
      NULL
    )));
  }
}

static void readrq (Machine *m) {
  IserverRq *rq = (IserverRq *)tk_pop_native(m, symbol_ISERVER_RQ_);
  char *msg = opt_nget(iserverRq_msg(rq));
  if (msg) {
    machine_push(m, token_new_list(arr_new_from(
      token_new_string(msg), NULL
    )));
  } else {
    machine_push(m, token_new_list(arr_new()));
  }
}

static void rqhost (Machine *m) {
  IserverRq *rq = (IserverRq *)tk_pop_native(m, symbol_ISERVER_RQ_);
  char *msg = opt_nget(iserverRq_msg(rq));
  if (msg) {
    machine_push(m, token_new_list(arr_new_from(
      token_new_string(iserverRq_host(rq)), NULL
    )));
  } else {
    machine_push(m, token_new_list(arr_new()));
  }
}

static void writerq (Machine *m) {
  IserverRq *rq = (IserverRq *)tk_pop_native(m, symbol_ISERVER_RQ_);
  char *msg = tk_pop_string(m);
  iserverRq_write(rq, msg);
}

Pmodule *modiserver_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("open", open); // [] - ISERVER
  add("close", close); // ISERVER - []
  add("getRq", getrq); // ISERVER - OPT<ISERVER_RQ>
  add("readRq", readrq); // ISERVER_RQ - OPT<STRING>
  add("rqHost", rqhost); // ISERVER_RQ - OPT<STRING>
  add("writeRq", writerq); // ISERVER_RQ - []

  return r;
}



