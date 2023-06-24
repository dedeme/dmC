// Copyright 05-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Quotes data base.

#ifndef DB_QUOTESDB_H
  #define DB_QUOTESDB_H

#include "kut/rs.h"
#include "kut/arr.h"

/// Initializes data base.
void quotesDb_init (void);

/// Returns Arr<char> of nicks.
Arr *quotesDb_nicks (void);

/// Reads content of a company file.
char *quotesDb_read (char *nick);

/// Reads Rs<Qs> content of a company file.
Rs *quotesDb_read_qs (char *nick);

/// Write content of a company file.
void quotesDb_write (char *nick, char *data);

#endif
