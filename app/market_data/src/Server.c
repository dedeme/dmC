// Copyright 12-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Server.h"
#include "dmc/Darr.h"
#include "io.h"
#include "servers/bolsamania.h"
#include "servers/eleconomista.h"
#include "servers/estrategias.h"
#include "servers/expansion.h"
#include "servers/finanzas.h"
#include "servers/infobolsa.h"
#include "servers/invertia.h"
#include "servers/libremercado.h"

/* .
-Server: serial
  id: char *
  active: int
  #Arr[char]
  nicks: Arr *: _a: _s
  #Quotes
  qs: Darr *: darr: darr
*/

/*--*/

struct server_Server {
  char *id;
  int active;
  Arr *nicks;
  Darr *qs;
};

Server *_server_new(
  char *id,
  int active,
  Arr *nicks,
  Darr *qs
) {
  Server *this = malloc(sizeof(Server));
  this->id = id;
  this->active = active;
  this->nicks = nicks;
  this->qs = qs;
  return this;
}

void server_free(Server *this) {
  free(this->id);
  arr_free(this->nicks);
  darr_free(this->qs);
  free(this);
};

char *server_id(Server *this) {
  return this->id;
}

int server_active(Server *this) {
  return this->active;
}

Arr *server_nicks(Server *this) {
  return this->nicks;
}

Darr *server_qs(Server *this) {
  return this->qs;
}

Js *server_to_js_new(Server *this) {
  // Arr[Js]
  Arr *a = arr_new(free);
  arr_push(a, js_ws_new(this->id));
  arr_push(a, js_wi_new(this->active));
  arr_push(a, arr_to_js_new(this->nicks, (FTO)js_ws_new));
  arr_push(a, darr_to_js_new(this->qs));
  Js *r = js_wa_new(a);
  arr_free(a);
  return r;
}

Server *server_from_js_new(Js *js) {
  // Arr[Js]
  Arr *a = js_ra_new(js);
  int i = 0;
  char *id = js_rs_new(arr_get(a, i++));
  int active = js_ri(arr_get(a, i++));
  Arr *nicks = arr_from_js_new(arr_get(a, i++), (FFROM)js_rs_new, free);
  Darr *qs = darr_from_js_new(arr_get(a, i++));
  arr_free(a);
  return _server_new(id, active, nicks, qs);
}

/*--*/

// Returns if reading is wrong
static int read_quotes(Varr **nicks_new, Darr **qs_new, char *id) {
  if (str_eq(id, bolsamania_name()))
    return bolsamania_read(nicks_new, qs_new);
  else if (str_eq(id, eleconomista_name()))
    return eleconomista_read(nicks_new, qs_new);
  else if (str_eq(id, estrategias_name()))
    return estrategias_read(nicks_new, qs_new);
  else if (str_eq(id, expansion_name()))
    return expansion_read(nicks_new, qs_new);
  else if (str_eq(id, finanzas_name()))
    return finanzas_read(nicks_new, qs_new);
  else if (str_eq(id, infobolsa_name()))
    return infobolsa_read(nicks_new, qs_new);
  else if (str_eq(id, invertia_name()))
    return invertia_read(nicks_new, qs_new);
  else if (str_eq(id, libremercado_name()))
    return libremercado_read(nicks_new, qs_new);
  else
    FAIL(str_f_new("Server '%s' not found", id));
  return 1;
}

static Server *new_null(char *id) {
  char *js = io_server_read_new(id);
  Server *r;
  if (!*js) {
    // Varr[char]
    Varr *vnicks;
    Darr *qs;
    if (read_quotes(&vnicks, &qs, id)) return NULL;
    // Arr[char]
    Arr *nicks = arr_new(free);
    EACH(vnicks, char, n)
      arr_push(nicks, str_new(n));
    _EACH
    varr_free(vnicks);

    r = _server_new(str_new(id), 0, nicks, qs);
    Js *js2 = server_to_js_new(r);
    io_server_write(id, (char *)js2);
    free(js2);
  } else {
    r = server_from_js_new((Js *)js);
  }
  free(js);
  return r;
}

void server_set_active(Server *this, int active) {
  this->active = active;
  Js *js2 = server_to_js_new(this);
  io_server_write(this->id, (char *)js2);
  free(js2);
}

void server_update(Server *this) {
  // Varr[char]
  Varr *vnicks;
  Darr *qs;
  read_quotes(&vnicks, &qs, this->id);
  // Arr[char]
  Arr *nicks = arr_new(free);
  int active = 0;
  EACH_IX(vnicks, char, n, ix)
    arr_push(nicks, str_new(n));
    if (darr_get(qs, ix) != server_quote(this, n)) {
      active = 1;
    }
  _EACH
  varr_free(vnicks);

  this->active = active;
  arr_free(this->nicks);
  this->nicks = nicks;
  darr_free(this->qs);
  this->qs = qs;

  Js *js2 = server_to_js_new(this);
  io_server_write(this->id, (char *)js2);
  free(js2);
}

double server_quote(Server *this, char *nick) {
  double r = -1;
  EACH_IX(this->nicks, char, n, ix)
    if (str_eq(n, nick)){
      r = darr_get(this->qs, ix);
      break;
    }
  _EACH
  return r;
}

int server_number(void) {
  return 8;
}

// Returns Arr[char]
static Arr *all_names_new(void) {
  char *names[] = {
    bolsamania_name(), eleconomista_name(), estrategias_name(),
    expansion_name(), finanzas_name(), infobolsa_name(),
    invertia_name(), libremercado_name(),
    NULL
  };

  char **p = names;

  // Varr[const_char]
  Arr *r = arr_new(free);
  while (*p) {
    arr_push(r, str_new(*p++));
  }
  return r;
}

// Arr[Server]
Arr *server_servers_new(void) {
  // Arr[Server]
  Arr *r = arr_new((FPROC) server_free);

  Arr *names = all_names_new();
  EACH(names, char, n)
    Server *sv = new_null(n);
    if (sv) arr_push(r, sv);
  _EACH

  return r;
}

Server *server_current_new() {
  char *server_name = io_server_current_read_new();
  Server *r = NULL;
  if (*server_name) {
    r = new_null(server_name);
  }
  if (!r) {
    r = server_next_new();
  }
  free(server_name);
  return r;
}

Server *server_next_new() {
  // Arr[char]
  Arr *box = io_servers_read_new();
  if (!arr_size(box)) {
    arr_free(box);
    box = all_names_new();
    arr_shuffle(box);
  }
  char *server_name = arr_get(box, 0);
  io_server_current_write(server_name);
  Server *r = new_null(server_name);
  arr_remove(box, 0);
  io_servers_write(box);
  arr_free(box);
  return r ? r : server_next_new();
}
