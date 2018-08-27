// Copyright 03-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "ast/Value.h"
#include "ast/Type.h"
#include "ast/Catt.h"

/*.
struct: Catt
  is_public: bool: _bool
  is_static: bool: _bool
  t: enum Catt_t: _int
  id: char *: _string
  type: Type *: type
  value: Value *: value
*/

/*.-.*/
struct catt_Catt {
  bool is_public;
  bool is_static;
  enum Catt_t t;
  char *id;
  Type *type;
  Value *value;
};

Catt *catt_new(
  bool is_public,
  bool is_static,
  enum Catt_t t,
  char *id,
  Type *type,
  Value *value
) {
  Catt *this = MALLOC(Catt);
  this->is_public = is_public;
  this->is_static = is_static;
  this->t = t;
  XNULL(id)
  this->id = id;
  XNULL(type)
  this->type = type;
  XNULL(value)
  this->value = value;
  return this;
}

bool catt_is_public(Catt *this) {
  XNULL(this)
  return this->is_public;
}

bool catt_is_static(Catt *this) {
  XNULL(this)
  return this->is_static;
}

enum Catt_t catt_t(Catt *this) {
  XNULL(this)
  return this->t;
}

char *catt_id(Catt *this) {
  XNULL(this)
  return this->id;
}

Type *catt_type(Catt *this) {
  XNULL(this)
  return this->type;
}

Value *catt_value(Catt *this) {
  XNULL(this)
  return this->value;
}

Ajson *catt_to_json(Catt *this) {
  XNULL(this)
  Ajson *serial = ajson_new();
  jarr_abool(serial, this->is_public);
  jarr_abool(serial, this->is_static);
  jarr_aint(serial, this->t);
  jarr_astring(serial, this->id);
  ajson_add(serial, json_warray(type_to_json(this->type)));
  ajson_add(serial, json_warray(value_to_json(this->value)));
  return serial;
}

Catt *catt_from_json(Ajson *serial) {
  XNULL(serial)
  Catt *this = MALLOC(Catt);
  size_t i = 0;
  this->is_public = jarr_gbool(serial, i++);
  this->is_static = jarr_gbool(serial, i++);
  this->t = jarr_gint(serial, i++);
  this->id = jarr_gstring(serial, i++);
  this->type = type_from_json(json_rarray(ajson_get(serial, i++)));
  this->value = value_from_json(json_rarray(ajson_get(serial, i++)));
  return this;
}
/*.-.*/
