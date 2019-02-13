// Copyright 11-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef SERVERS_H
  #define SERVERS_H

#include "dmc/std.h"
#include "dmc/Darr.h"

/// Returns an Arr[char] with every server
Arr *severs_all_names_new(void);

/// Returns an Arr[char] with selected servers
Arr *servers_all_selected_names_new(void);

/// Returns data of a a server:
///   url_new: Return the server url
///   codes_new: It is a Map[String]: nicks -> codes
///   server: Name of server
void servers_data(char **url_new, Map **codes_new, char *server);

/// Returns quotes of a server
///   nicks_new (return)(Arr[char]): Nicks list
///   qs_new (return): each quote of nicks_new
///   server: Name of server
///   return: 1 if reading is wrong
int servers_read_quotes(Varr **nicks_new, Darr **qs_new, char *server);

/// Tests servers
void servers_tests (void);

#endif
