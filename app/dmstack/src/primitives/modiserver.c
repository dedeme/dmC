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
  Token **values = ATOMIC(sizeof(Token *) + sizeof(Token *));
  if (*err) {
    values[0] = token_new_string(err);
    values[1] = NULL;
    machine_push(m, token_from_pointer(symbol_EITHER_, values));
  } else {
    values[0] = NULL;
    values[1] = token_from_pointer(symbol_ISERVER_RQ_, rq);
    machine_push(m, token_from_pointer(symbol_EITHER_, values));
  }
}

static void readrq (Machine *m) {
  IserverRq *rq = (IserverRq *)tk_pop_native(m, symbol_ISERVER_RQ_);
  char *msg = opt_nget(iserverRq_msg(rq));
  if (msg) {
    machine_push(m, token_from_pointer(
      symbol_OPTION_, token_new_string(msg)
    ));
  } else {
    machine_push(m, token_from_pointer(symbol_OPTION_, NULL));
  }
}

static void rqhost (Machine *m) {
  IserverRq *rq = (IserverRq *)tk_pop_native(m, symbol_ISERVER_RQ_);
  char *msg = opt_nget(iserverRq_msg(rq));
  if (msg) {
    machine_push(m, token_from_pointer(
      symbol_OPTION_, token_new_string(iserverRq_host(rq))
    ));
  } else {
    machine_push(m, token_from_pointer(symbol_OPTION_, NULL));
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
  add("getRq", getrq); // ISERVER - EITHER<STRING, ISERVER_RQ>
  add("readRq", readrq); // ISERVER_RQ - OPT<STRING>
  add("rqHost", rqhost); // ISERVER_RQ - OPT<STRING>
  add("writeRq", writerq); // ISERVER_RQ - []

  return r;
}



