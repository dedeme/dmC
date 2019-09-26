// Copyright 23-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modiserver.h"
#include "dmc/Iserver.h"
#include "fails.h"
#include "Machine.h"

static void open (Machine *m) {
  int port = token_int(machine_pop_exc(m, token_INT));
  machine_push(m, token_from_pointer("= __Iserver", iserver_new(port)));
}

static void close (Machine *m) {

  Iserver *s = (Iserver *)fails_read_pointer(m, "= __Iserver", machine_pop(m));
  iserver_close(s);
}

static void getrq (Machine *m) {
  Iserver *s = (Iserver *)fails_read_pointer(m, "= __Iserver", machine_pop(m));
  IserverRq *rq = iserver_read(s);
  char *err = iserverRq_error(rq);
  if (*err) {
    machine_push(m, token_new_list(0, arr_new_from(
      token_new_string(0, err), token_new_int(0, 0), NULL
    )));
  } else {
    machine_push(m, token_new_list(0, arr_new_from(
      token_from_pointer("= __IserveRq", rq),
      NULL
    )));
  }
}

static void readrq (Machine *m) {
  IserverRq *rq = (IserverRq *)fails_read_pointer(
    m, "= __IserverRq", machine_pop(m)
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
    m, "= __IserverRq", machine_pop(m)
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
    m, "= __IserverRq", machine_pop(m)
  );
  char *msg = token_string(machine_pop_exc(m, token_STRING));
  iserverRq_write(rq, msg);
}

// Resturns Map<primitives_Fn>
Map *modiserver_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "open", open); // [] - ISERVER
  map_put(r, "close", close); // ISERVER - []
  map_put(r, "getRq", getrq); // ISERVER - OPT<ISERVER_RQ>
  map_put(r, "readRq", readrq); // ISERVER_RQ - OPT<STRING>
  map_put(r, "rqHost", rqhost); // ISERVER_RQ - OPT<STRING>
  map_put(r, "writeRq", writerq); // ISERVER_RQ - []

  return r;
}



