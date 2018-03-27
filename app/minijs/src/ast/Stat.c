// Copyright 25-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Stat.h"

/*.
struct: Stat
  stype: enum Stat_t: _uint
*/

/*.-.*/
struct stat_Stat {
  enum Stat_t stype;
};

Stat *stat_new(enum Stat_t stype) {
  Stat *this = MALLOC(Stat);
  this->stype = stype;
  return this;
}

inline
enum Stat_t stat_stype(Stat *this) {
  return this->stype;
}

Json *stat_serialize(Stat *this) {
  Arr/*Json*/ *serial = arr_new();
  jarr_auint(serial, this->stype);
  return json_warray(serial);
}

Stat *stat_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  Stat *this = MALLOC(Stat);
  size_t i = 0;
  this->stype = jarr_guint(serial, i++);
  return this;
}
/*.-.*/

bool stat_eq(Stat *this, Stat *other) {
  if (this->stype != other->stype) {
    return false;
  }
  return true;
}
