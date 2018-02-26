// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Data base

#ifndef DB_H
  # define DB_H

#include "Nick.h"
#include "Flea.h"

///
typedef struct db_Db Db;

///
Db *db_new(void);

///
size_t db_next_flea_id(Db *this);

///
size_t db_cycle(Db *this);

///
void db_inc_cycle(Db *this);

///
Fleas *db_fleas(Db *this);

///
Arr/*char*/ *db_qdates(Db *this);

///
Quote **db_quotes(Db *this);

///
Nicks *db_nicks(Db *this);

///
void db_save(Db *this);

#endif

