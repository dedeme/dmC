// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Nick.h"

struct nick_Nick {
  char *id;
  bool in_ibex;
};

Nick *nick_new(char *id) {
  Nick *this = MALLOC(Nick);
  this->id = id;
  this->in_ibex = false;
  return this;
}

inline
char *nick_id(Nick *this) {
  return this->id;
}

inline
bool nick_in_ibex(Nick *this) {
  return this->in_ibex;
}

inline
void nick_set_in_ibex(Nick *this) {
  this->in_ibex = true;
}

struct nicks_nicks {
  Nick **nicks;
  size_t size;
};

Nicks *nicks_new(size_t size) {
  Nicks *this = MALLOC(Nicks);
  this->nicks = GC_MALLOC(sizeof(Nick) * size);
  this->size = size;
  return this;
}

inline
size_t nicks_size(Nicks *this) {
  return this->size;
}

inline
void nicks_set(Nicks *this, size_t ix, Nick *nick) {
  *(this->nicks + ix) = nick;
}

inline
Nick *nicks_get(Nicks *this, size_t ix) {
  return *(this->nicks + ix);
}
