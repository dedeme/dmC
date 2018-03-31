// Copyright 25-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Stat.h"

/*.
-struct: @Stat
  stype: enum Stat_t
  data: void *
*/

/*.-.*/
struct stat_Stat {
  enum Stat_t stype;
  void *data;
};

Stat *_stat_new(enum Stat_t stype, void *data) {
  Stat *this = MALLOC(Stat);
  this->stype = stype;
  this->data = data;
  return this;
}

inline
enum Stat_t stat_stype(Stat *this) {
  return this->stype;
}

inline
void *stat_data(Stat *this) {
  return this->data;
}
/*.-.*/

inline
Stat *stat_new_val(Dvalue *value) {
  return _stat_new(SVAL, value);
}

Json *stat_serialize(Stat *this) {
  Arr/*Json*/ *serial = arr_new();
  jarr_auint(serial, this->stype);
  switch (this->stype) {
  case SVAL:
    arr_add(serial, dvalue_serialize(this->data));
    break;
  default:
    THROW "Unknown type '%d'", this->stype _THROW
  }

  return json_warray(serial);
}

Stat *stat_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  Stat *this = MALLOC(Stat);
  size_t i = 0;
  this->stype = jarr_guint(serial, i++);

  switch (this->stype) {
  case SVAL:
    this->data = dvalue_restore(arr_get(serial, i));
    break;
  default:
    THROW "Unknown type '%d'", this->stype _THROW
  }

  return this;
}
