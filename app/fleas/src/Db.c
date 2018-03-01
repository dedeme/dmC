// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Db.h"
#include <dm/dm.h>
#include "io.h"
#include "DEFS.h"

struct db_Db {
  size_t fleaId;
  size_t cycle;

  Fleas *fleas;

  Nicks *nicks;
  Arr/*char*/ *qdates;
  Quote **quotes;
};

Db *db_new(void) {
  Db *this = MALLOC(Db);

  this->nicks = io_nicks();
  if (nicks_size(this->nicks) != NICKS_NUMBER) {
    THROW
      "this->nicks has %d elements, but ought to have %d",
      nicks_size(this->nicks), NICKS_NUMBER
    _THROW
  }

  this->qdates = io_quotes(this->nicks);
  this->quotes = quotes_get();

  io_get_fleas(&this->fleaId, &this->cycle, &this->fleas);

  return this;
}

inline
size_t db_next_flea_id(Db *this) {
  return this->fleaId++;
}

inline
size_t db_cycle(Db *this) {
  return this->cycle;
}

inline
void db_inc_cycle(Db *this) {
  ++this->cycle;
}

inline
Fleas *db_fleas(Db *this) {
  return this->fleas;
}

inline
Arr/*char*/ *db_qdates(Db *this) {
  return this->qdates;
}

inline
Quote **db_quotes(Db *this) {
  return this->quotes;
}

inline
Nicks *db_nicks(Db *this) {
  return this->nicks;
}

inline
void db_save(Db *this) {
  io_set_fleas(this->fleaId, this->cycle, this->fleas);
}
