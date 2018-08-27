// Copyright 10-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "ast/Vatt.h"
#include "ast/Value.h"
#include "ct/Lvalue.h"
#include "lexer/Pos.h"

/*.
struct: @Vatt
  is_fn: bool: _bool
  pos: Pos *: pos
  id: char *: _string
  params: Lvalue *: __lvalue - value
*/

/*.-.*/
struct vatt_Vatt {
  bool is_fn;
  Pos *pos;
  char *id;
  Lvalue *params;
};

Vatt *_vatt_new(
  bool is_fn,
  Pos *pos,
  char *id,
  Lvalue *params
) {
  Vatt *this = MALLOC(Vatt);
  this->is_fn = is_fn;
  XNULL(pos)
  this->pos = pos;
  XNULL(id)
  this->id = id;
  XNULL(params)
  this->params = params;
  return this;
}

bool vatt_is_fn(Vatt *this) {
  XNULL(this)
  return this->is_fn;
}

Pos *vatt_pos(Vatt *this) {
  XNULL(this)
  return this->pos;
}

char *vatt_id(Vatt *this) {
  XNULL(this)
  return this->id;
}

Lvalue *vatt_params(Vatt *this) {
  XNULL(this)
  return this->params;
}

Ajson *vatt_to_json(Vatt *this) {
  XNULL(this)
  Ajson *serial = ajson_new();
  jarr_abool(serial, this->is_fn);
  ajson_add(serial, json_warray(pos_to_json(this->pos)));
  jarr_astring(serial, this->id);
  ajson_add(serial, json_warray(lvalue_to_json(this->params, value_to_json)));
  return serial;
}

Vatt *vatt_from_json(Ajson *serial) {
  XNULL(serial)
  Vatt *this = MALLOC(Vatt);
  size_t i = 0;
  this->is_fn = jarr_gbool(serial, i++);
  this->pos = pos_from_json(json_rarray(ajson_get(serial, i++)));
  this->id = jarr_gstring(serial, i++);
  this->params = lvalue_from_json(json_rarray(ajson_get(serial, i++)), value_from_json);
  return this;
}
/*.-.*/

Vatt *vatt_new_id(Pos *pos, char *id) {
  XNULL(pos)
  XNULL(id)
  return _vatt_new(false, pos, id, lvalue_new());
}

Vatt *vatt_new_fn(Pos *pos, Lvalue *vs) {
  XNULL(pos)
  XNULL(vs)
  return _vatt_new(true, pos, "", vs);
}
