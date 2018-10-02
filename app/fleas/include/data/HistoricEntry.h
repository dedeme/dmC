// Copyright 29-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef DATA_HISTORICENTRY_H
  #define DATA_HISTORICENTRY_H

#include "dmc/std.h"
#include "Flea.h"

/*.-.*/

#include "dmc/Json.h"

///
typedef struct historicEntry_HistoricEntry HistoricEntry;

///
HistoricEntry *historicEntry_new(
  Flea *flea,
  double pond,
  double assets,
  int buys,
  int sells
);

///
Flea *historicEntry_flea(HistoricEntry *this);

///
double historicEntry_pond(HistoricEntry *this);

///
double historicEntry_assets(HistoricEntry *this);

///
int historicEntry_buys(HistoricEntry *this);

///
int historicEntry_sells(HistoricEntry *this);

///
Json *historicEntry_to_json(HistoricEntry *this);

///
HistoricEntry *historicEntry_from_json(Json *s);

/*.-.*/

#endif
