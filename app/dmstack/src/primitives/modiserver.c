// Copyright 23-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modiserver.h"
#include "dmc/Iserver.h"
#include "fails.h"
#include "Machine.h"

static void open (Machine *m) {
  int port = token_int(machine_pop_exc(m, token_INT));
  machine_push(m, token_from_pointer(symbol_ISERVER_, iserver_new(port)));
}

static void close (Machine *m) {

  Iserver *s = (Iserver *)fails_read_pointer(m, symbol_ISERVER_, machine_pop(m));
  iserver_close(s);
}

static void getrq (Machine *m) {
  Iserver *s = (Iserver *)fails_read_pointer(m, symbol_ISERVER_, machine_pop(m));
  IserverRq *rq = iserver_read(s);
  char *err = iserverRq_error(rq);
  if (*err) {
    machine_push(m, token_new_list(0, arr_new_from(
      token_new_string(0, err), token_new_int(0, 0), NULL
    )));
  } else {
    machine_push(m, token_new_list(0, arr_new_from(
      token_from_pointer(symbol_ISERVER_RQ_, rq),
      NULL
    )));
  }
}

static void readrq (Machine *m) {
  IserverRq *rq = (IserverRq *)fails_read_pointer(
    m, symbol_ISERVER_RQ_, machine_pop(m)
  );
  char *msg = opt_nget(iserverRq_msg(rq));
  if (msg) {
    machine_push(m, token_new_list(0, arr_new_from(
      token_new_string(0, msg), NULL
    )));
  } else {
    machine_push(m, token_new_list(0, arr_new()));
  }
}

static void rqhost (Machine *m) {
  IserverRq *rq = (IserverRq *)fails_read_pointer(
    m, symbol_ISERVER_RQ_, machine_pop(m)
  );
  char *msg = opt_nget(iserverRq_msg(rq));
  if (msg) {
    machine_push(m, token_new_list(0, arr_new_from(
      token_new_string(0, iserverRq_host(rq)), NULL
    )));
  } else {
    machine_push(m, token_new_list(0, arr_new()));
  }
}

static void writerq (Machine *m) {
  IserverRq *rq = (IserverRq *)fails_read_pointer(
    m, symbol_ISERVER_RQ_, machine_pop(m)
  );
  char *msg = token_string(machine_pop_exc(m, token_STRING));
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



