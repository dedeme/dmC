// Copyright 06-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Companies server codes table.

#ifndef DB_SV_CODES_TB_H
  #define DB_SV_CODES_TB_H

#include "kut/map.h"

/// Initialize table.
void svCodesTb_init (void);

/// Returns a Map<char>[nick, code] with company codes of 'server_id'.
/// If such map does not exist, it is created.
Map *svCodesTb_read (char *server_id);

/// Write a Map<char>[nick, code] in 'server_id'
void svCodesTb_write (char *server_id, Map *nk_codes);

/// Remove maps of not existing servers.
void svCodesTb_purge (void);

#endif
