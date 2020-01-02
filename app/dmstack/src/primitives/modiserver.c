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
  Token **values = GC_MALLOC(sizeof(Token *) + sizeof(Token *));
  values[0] = NULL;
  values[1] = NULL;
  if (*err)
    values[0] = token_new_string(err);
  else
    values[1] = token_from_pointer(symbol_ISERVER_RQ_, rq);

  machine_push(m, token_from_pointer(symbol_EITHER_, values));
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

static void writerp (Machine *m) {
  char *msg = tk_pop_string(m);
  IserverRq *rq = (IserverRq *)tk_pop_native(m, symbol_ISERVER_RQ_);
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
  add("writeRp", writerp); // [ISERVER_RQ, STRING] - []

  return r;
}



