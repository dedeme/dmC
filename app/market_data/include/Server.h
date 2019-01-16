// Copyright 12-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Server data and operations.

#ifndef SERVER_H
  #define SERVER_H

#include "dmc/std.h"
#include "dmc/Darr.h"

/*--*/

///
typedef struct server_Server Server;

///
void server_free(Server *this);

///
char *server_id(Server *this);

///
int server_active(Server *this);

/// Arr[char]
Arr *server_nicks(Server *this);

/// Quotes
Darr *server_qs(Server *this);

///
Js *server_to_js_new(Server *this);

///
Server *server_from_js_new(Js *js);

/*--*/

/// Changes active: 0 -> sleep, 1 -> active
void server_set_active(Server *this, int active);

/// Updates server
void server_update(Server *this);

/// Returns quote of 'nick' or -1 if nick is not found.
double server_quote(Server *this, char *nick);

/// Return servers number.
int server_number(void);

/// Returns all servers. This function read data from disk.
Arr *server_servers_new(void);

/// Returns the current server
Server *server_current_new();

/// Selects a randomly server.
Server *server_next_new();

#endif
