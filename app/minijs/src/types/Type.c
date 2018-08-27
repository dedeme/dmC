// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/Type.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
struct: Type
  is_function: bool: _bool
  id: char *: _string
  params: Atype *: _array type
*/

/*.-.*/
struct type_Type {
  bool is_function;
  char *id;
  Atype *params;
};

Type *type_new(bool is_function, char *id, Atype *params) {
  Type *this = MALLOC(Type);
  this->is_function = is_function;
  XNULL(id)
  this->id = id;
  XNULL(params)
  this->params = params;
  return this;
}

bool type_is_function(Type *this) {
  XNULL(this)
  return this->is_function;
}

char *type_id(Type *this) {
  XNULL(this)
  return this->id;
}

Atype *type_params(Type *this) {
  XNULL(this)
  return this->params;
}

Json *type_to_json(Type *this) {
  XNULL(this)
  Ajson *serial = ajson_new();
  jarr_abool(serial, this->is_function);
  jarr_astring(serial, this->id);
  jarr_aarray(serial, (Arr *)this->params, (Json*(*)(void*)) type_to_json);
  return json_warray(serial);
}

Type *type_from_json(Json *js) {
  XNULL(js)
  Ajson *serial = json_rarray(js);
  Type *this = MALLOC(Type);
  size_t i = 0;
  this->is_function = jarr_gbool(serial, i++);
  this->id = jarr_gstring(serial, i++);
  this->params = (Atype *)jarr_garray(serial, i++, (void*(*)(Json*)) type_from_json);
  return this;
}
/*.-.*/

bool type_eq(Type *this, Type *other) {
  bool isf = this->is_function;
  if (isf != other->is_function) {
    return false;
  }

  Atype *tparams = this->params;
  Atype *oparams = other->params;

  size_t size = atype_size(tparams);
  if (size != atype_size(oparams)) {
    return false;
  }

  RANGE0(i, size) {
    if (!type_eq(atype_get(tparams, i), atype_get(oparams, i))) {
      return false;
    }
  }_RANGE

  if (!isf && !str_eq(this->id, other->id)) {
    return false;
  }

  return true;
}
