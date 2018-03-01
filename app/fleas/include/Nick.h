// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Nick data

#ifndef NICK_H
  # define NICK_H

#include <dm/dm.h>

///
typedef struct nick_Nick Nick;

/// nick_new creates a new Nick. 'id' will be deleted when 'nick_free' will be
/// called.
Nick *nick_new(char *id);

char *nick_id(Nick *this);

///
bool nick_in_ibex(Nick *this);

///
void nick_set_in_ibex(Nick *this);

///
typedef struct nicks_nicks Nicks;

///
Nicks *nicks_new(size_t size);

///
size_t nicks_size(Nicks *this);

/// nicks_set adds a 'nick' to 'this'. This nick will be deleted when
/// 'nicks_free' be called.It does control limits.
void nicks_set(Nicks *this, size_t ix, Nick *nick);

/// nicks_get returns a reference. It does control limits.
Nick *nicks_get(Nicks *this, size_t ix);

#endif
