// Copyright 25-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Stat.h"

/*.
-struct: @Stat
  pos: Pos *
  stype: enum Stat_t
  data: void *
*/

/*.-.*/
struct stat_Stat {
  Pos *pos;
  enum Stat_t stype;
  void *data;
};

Stat *_stat_new(Pos *pos, enum Stat_t stype, void *data) {
  Stat *this = MALLOC(Stat);
  this->pos = pos;
  this->stype = stype;
  this->data = data;
  return this;
}

inline
Pos *stat_pos(Stat *this) {
  return this->pos;
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
Stat *stat_new_val(Pos *pos, Dvalue *value) {
  return _stat_new(pos, SVAL, value);
}

inline
Stat *stat_new_var(Pos *pos, Dvalue *value) {
  return _stat_new(pos, SVAR, value);
}

inline
Stat *stat_new_fn(Pos *pos, Arr/*Value*/ *values) {
  return _stat_new(pos, SFN, values);
}

inline
Stat *stat_new_return(Pos *pos, Value *value) {
  return _stat_new(pos, SRETURN, value);
}

Json *stat_serialize(Stat *this) {
  Arr/*Json*/ *serial = arr_new();
  arr_add(serial, pos_serialize(this->pos));
  jarr_auint(serial, this->stype);
  switch (this->stype) {
  case SVAL:
  case SVAR:
    arr_add(serial, dvalue_serialize(this->data));
    break;
  case SFN:
    jarr_aarray(serial, this->data, (Json *(*)(void *))value_serialize);
    break;
  case SRETURN:
    arr_add(serial, value_serialize(this->data));
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
  this->pos = pos_restore(arr_get(serial, i++));
  this->stype = jarr_guint(serial, i++);

  switch (this->stype) {
  case SVAL:
  case SVAR:
    this->data = dvalue_restore(arr_get(serial, i));
    break;
  case SFN:
    this->data = jarr_garray(serial, i, (void *(*)(Json *))value_restore);
    break;
  case SRETURN:
    this->data = value_restore(arr_get(serial, i));
    break;
  default:
    THROW "Unknown type '%d'", this->stype _THROW
  }

  return this;
}
