// Copyright 29-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/HistoricEntry.h"

/* .+.
struct: HistoricEntry
  flea: Flea *: flea
  pond: double: _double 2
  assets: double: _double 2
  buys: int: _int
  sells: int: _int
*/

/*.-.*/
#include "dmc/ct/Ajson.h"

struct historicEntry_HistoricEntry {
  Flea *flea;
  double pond;
  double assets;
  int buys;
  int sells;
};

HistoricEntry *historicEntry_new(
  Flea *flea,
  double pond,
  double assets,
  int buys,
  int sells
) {
  HistoricEntry *this = MALLOC(HistoricEntry);
  XNULL(flea)
  this->flea = flea;
  this->pond = pond;
  this->assets = assets;
  this->buys = buys;
  this->sells = sells;
  return this;
}

Flea *historicEntry_flea(HistoricEntry *this) {
  XNULL(this)
  return this->flea;
}

double historicEntry_pond(HistoricEntry *this) {
  XNULL(this)
  return this->pond;
}

double historicEntry_assets(HistoricEntry *this) {
  XNULL(this)
  return this->assets;
}

int historicEntry_buys(HistoricEntry *this) {
  XNULL(this)
  return this->buys;
}

int historicEntry_sells(HistoricEntry *this) {
  XNULL(this)
  return this->sells;
}

Json *historicEntry_to_json(HistoricEntry *this) {
  XNULL(this)
  Ajson *serial = ajson_new();
  ajson_add(serial, flea_to_json(this->flea));
  jarr_adouble(serial, this->pond,  2);
  jarr_adouble(serial, this->assets,  2);
  jarr_aint(serial, this->buys);
  jarr_aint(serial, this->sells);
  return json_warray(serial);
}

HistoricEntry *historicEntry_from_json(Json *js) {
  XNULL(js)
  Ajson *serial = json_rarray(js);
  HistoricEntry *this = MALLOC(HistoricEntry);
  size_t i = 0;
  this->flea = flea_from_json(ajson_get(serial, i++));
  this->pond = jarr_gdouble(serial, i++);
  this->assets = jarr_gdouble(serial, i++);
  this->buys = jarr_gint(serial, i++);
  this->sells = jarr_gint(serial, i++);
  return this;
}
/*.-.*/
