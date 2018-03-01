// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Portfolio management

#ifndef PORTFOLIO_H
  # define PORTFOLIO_H

#include <stdlib.h>
#include <Nick.h>
#include <dm/Json.h>

///
typedef struct portfolio_Pentry Pentry;

///
size_t portfolio_nick(Pentry *this);

///
size_t portfolio_stocks(Pentry *this);

///
typedef Arr/*Pentry*/ Portfolio;

///
Portfolio *portfolio_new(void);

///
Portfolio *portfolio_copy(Portfolio *this);

///
void portfolio_add(Portfolio *this, size_t nick, size_t stocks);

/// portfolio_get returns the stock number of a nick.
size_t portfolio_get(Portfolio *this, size_t nick);

///
void portfolio_remove(Portfolio *this, size_t nick, size_t stocks);

///
Json *portfolio_serialize(Portfolio *this, Nicks *nicks);

#endif


