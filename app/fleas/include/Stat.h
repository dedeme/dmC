// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Statistical data

#ifndef STAT_H
  # define STAT_H

#include <stdlib.h>
#include <dm/Json.h>

///
typedef struct gen_Stat Stat;

///
Stat *stat_new(void);

///
Stat *stat_copy(Stat *this);

///
double stat_cash(Stat *this);

///
double stat_buys(Stat *this);

///
double stat_sells(Stat *this);

/// 'n' is the cycles number of a Flea
void stat_setup(Stat *this, uint n, double cash, double Nbuys, double Nsells);

///
Json *stat_serialize(Stat *this);

///
Stat *stat_restore(Json *serial);
#endif



