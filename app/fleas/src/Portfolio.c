// Copyright 17-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <dm.h>
#include "Portfolio.h"

struct portfolio_Pentry {
  size_t nick;
  size_t stocks;
};

static Pentry *portfolio_Pentry_new(size_t nick, size_t stocks) {
  Pentry *this = MALLOC(Pentry);
  this->nick = nick;
  this->stocks = stocks;
  return this;
}

inline
size_t portfolio_nick(Pentry *this) {
  return this->nick;
}

inline
size_t portfolio_stocks(Pentry *this) {
  return this->stocks;
}

inline
Portfolio *portfolio_new(void) {
  return arr_new();
}

inline
Portfolio *portfolio_copy(Portfolio *this) {
  /**/FNM(copy, Pentry, e) {
  /**/  return portfolio_Pentry_new(e->nick, e->stocks);
  /**/}_FN
  return it_to(it_map(it_from(this), copy));
}

void portfolio_add(Portfolio *this, size_t nick, size_t stocks) {
  bool new = true;
  EACH(this, Pentry, e) {
    if (e->nick == nick) {
      e->stocks += stocks;
      new = false;
      break;
    }
  }_EACH
  if (new) {
    arr_add(this, portfolio_Pentry_new(nick, stocks));
  }
}

size_t portfolio_get(Portfolio *this, size_t nick) {
  EACH(this, Pentry, e) {
    if (e->nick == nick) {
      return e->stocks;
    }
  }_EACH
  return 0;
}

void portfolio_remove(Portfolio *this, size_t nick, size_t stocks) {
  size_t n = 0;
  bool error = true;
  int index = -1;
  EACH(this, Pentry, e) {
    if (e->nick == nick) {
      if (e->stocks == stocks) {
        index = _i;
        error = false;
      } else if (e->stocks >= stocks) {
        e->stocks -= stocks;
        error = false;
      } else {
        n = e->stocks;
      }
      break;
    }
  }_EACH
  if (error) {
    THROW
      "Try of selling %d stocks when only there are %d in portfolio",
      stocks, n
    _THROW
  }
  if (index != -1) {
    arr_remove(this, index);
  }
}

Json *portfolio_serialize(Portfolio *this, Nicks *nicks) {
  Arr/*Json*/ *jr = arr_new();

  EACH(this, Pentry, pe) {
    char *nick = nick_id(nicks_get(nicks, pe->nick));

    Arr/*Json*/ *ajs = arr_new();
    jarr_astring(ajs, nick);
    jarr_auint(ajs, pe->stocks);

    jarr_aarray(jr, ajs);
  }_EACH

  return json_warray(jr);
}
