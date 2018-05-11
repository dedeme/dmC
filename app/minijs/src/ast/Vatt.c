// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Vatt.h"
#include "ast/Avalue.h"

/*.
struct: @Vatt
  is_fn: bool: _bool
  pos: Pos *: pos
  id: char *: _string
  params: Avalue *: avalue
*/

/*.-.*/
struct vatt_Vatt {
  bool is_fn;
  Pos *pos;
  char *id;
  Avalue *params;
};

Vatt *_vatt_new(
  bool is_fn,
  Pos *pos,
  char *id,
  Avalue *params
) {
  Vatt *this = MALLOC(Vatt);
  this->is_fn = is_fn;
  this->pos = pos;
  this->id = id;
  this->params = params;
  return this;
}

inline
bool vatt_is_fn(Vatt *this) {
  return this->is_fn;
}

inline
Pos *vatt_pos(Vatt *this) {
  return this->pos;
}

inline
char *vatt_id(Vatt *this) {
  return this->id;
}

inline
Avalue *vatt_params(Vatt *this) {
  return this->params;
}

Json *vatt_serialize(Vatt *this) {
  if (!this) return json_wnull();
  Arr/*Json*/ *serial = arr_new();
  jarr_abool(serial, this->is_fn);
  arr_add(serial, pos_serialize(this->pos));
  jarr_astring(serial, this->id);
  arr_add(serial, avalue_serialize(this->params));
  return json_warray(serial);
}

Vatt *vatt_restore(Json *s) {
  if (json_rnull(s)) return NULL;
  Arr/*Json*/ *serial = json_rarray(s);
  Vatt *this = MALLOC(Vatt);
  size_t i = 0;
  this->is_fn = jarr_gbool(serial, i++);
  this->pos = pos_restore(arr_get(serial, i++));
  this->id = jarr_gstring(serial, i++);
  this->params = avalue_restore(arr_get(serial, i++));
  return this;
}
/*.-.*/

inline
Vatt *vatt_new_id(Pos *pos, char *id) {
  return _vatt_new(false, pos, id, arr_new());
}

inline
Vatt *vatt_new_fn(Pos *pos, Avalue *vs) {
  return _vatt_new(true, pos, "", vs);
}
