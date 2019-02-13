// Copyright 13-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef SERVERS_ELECONOMISTA_H
  #define SERVERS_ELECONOMISTA_H

#include "dmc/std.h"
#include "dmc/Darr.h"

/// Returns the eleconomista server name.
char *eleconomista_name(void);

// ONLY FOR DEBUG
// eleconomista_read_raw works the same as 'read' but returning server codes
// instead nicks.
// Returns 1 if reading is wrong, otherwise returns 0
int eleconomista_read_raw(Arr **codes_new, Darr **qs_new, char *url);

/// 'eleconomista_read' reads current quotes in new pairs Varr[char] - Darr.<p>
/// Each nick of nicks_new match with the quote of 'qs_new' with the same
/// index.<p>
/// Returns 1 if reading is wrong, otherwise returns 0
int eleconomista_read(Varr **nicks_new, Darr **qs_new);

#endif
