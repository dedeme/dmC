// Copyright 26-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Cvalue.h"

/*.
struct: @Cvalue
  pos: Pos *: pos
  is_public: bool: _bool
  is_static: bool: _bool
  type: enum Cvalue_t: _uint
  dvalue: Dvalue *: dvalue
*/

/*.-.*/
struct cvalue_Cvalue {
  Pos *pos;
  bool is_public;
  bool is_static;
  enum Cvalue_t type;
  Dvalue *dvalue;
};

Cvalue *_cvalue_new(
  Pos *pos,
  bool is_public,
  bool is_static,
  enum Cvalue_t type,
  Dvalue *dvalue
) {
  Cvalue *this = MALLOC(Cvalue);
  this->pos = pos;
  this->is_public = is_public;
  this->is_static = is_static;
  this->type = type;
  this->dvalue = dvalue;
  return this;
}

inline
Pos *cvalue_pos(Cvalue *this) {
  return this->pos;
}

inline
bool cvalue_is_public(Cvalue *this) {
  return this->is_public;
}

inline
bool cvalue_is_static(Cvalue *this) {
  return this->is_static;
}

inline
enum Cvalue_t cvalue_type(Cvalue *this) {
  return this->type;
}

inline
Dvalue *cvalue_dvalue(Cvalue *this) {
  return this->dvalue;
}

Json *cvalue_serialize(Cvalue *this) {
  Arr/*Json*/ *serial = arr_new();
  arr_add(serial, pos_serialize(this->pos));
  jarr_abool(serial, this->is_public);
  jarr_abool(serial, this->is_static);
  jarr_auint(serial, this->type);
  arr_add(serial, dvalue_serialize(this->dvalue));
  return json_warray(serial);
}

Cvalue *cvalue_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  Cvalue *this = MALLOC(Cvalue);
  size_t i = 0;
  this->pos = pos_restore(arr_get(serial, i++));
  this->is_public = jarr_gbool(serial, i++);
  this->is_static = jarr_gbool(serial, i++);
  this->type = jarr_guint(serial, i++);
  this->dvalue = dvalue_restore(arr_get(serial, i++));
  return this;
}
/*.-.*/

inline
Cvalue *cvalue_new_val(
  Pos *pos,
  bool is_public,
  bool is_static,
  Dvalue *dvalue
) {
  return _cvalue_new(pos, is_public, is_static, VAL, dvalue);
}

inline
Cvalue *cvalue_new_var(
  Pos *pos,
  bool is_public,
  bool is_static,
  Dvalue *dvalue
) {
  return _cvalue_new(pos, is_public, is_static, VAR, dvalue);
}

inline
char *cvalue_id(Cvalue *this) {
  if (!this->dvalue)
    THROW "This is not a Dvalue" _THROW

  return dvalue_id(this->dvalue);
}
