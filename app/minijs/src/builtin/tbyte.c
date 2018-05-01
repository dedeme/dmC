// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "builtin/tbyte.h"

void tbyte_add(Arr/*class*/ *builtins) {
  Class *c = class_new("Byte");
  Aatt *statics = class_statics(c);

  Atype *params = atype_new();
  atype_add(params, type_new_byte());
  atype_add(params, type_new_byte());
  Type *unary = type_new_fn(params);
  aatt_add(statics, att_new("minus", unary, true, NULL));
  aatt_add(statics, att_new("rinc", unary, true, NULL));
  aatt_add(statics, att_new("rdec", unary, true, NULL));
  aatt_add(statics, att_new("rnd", unary, true, NULL));

  params = atype_new();
  atype_add(params, type_new_byte());
  atype_add(params, type_new_void());
  unary = type_new_fn(params);
  aatt_add(statics, att_new("linc", unary, true, NULL));
  aatt_add(statics, att_new("ldec", unary, true, NULL));

  params = atype_new();
  atype_add(params, type_new_byte());
  atype_add(params, type_new_byte());
  atype_add(params, type_new_bool());
  Type *binary = type_new_fn(params);

  aatt_add(statics, att_new("eq", binary, true, NULL));
  aatt_add(statics, att_new("neq", binary, true, NULL));
  aatt_add(statics, att_new("lt", binary, true, NULL));
  aatt_add(statics, att_new("gt", binary, true, NULL));
  aatt_add(statics, att_new("leq", binary, true, NULL));
  aatt_add(statics, att_new("geq", binary, true, NULL));

  params = atype_new();
  atype_add(params, type_new_byte());
  atype_add(params, type_new_byte());
  atype_add(params, type_new_byte());
  binary = type_new_fn(params);

  aatt_add(statics, att_new("plus", binary, true, NULL));
  aatt_add(statics, att_new("sub", binary, true, NULL));
  aatt_add(statics, att_new("mul", binary, true, NULL));
  aatt_add(statics, att_new("div", binary, true, NULL));
  aatt_add(statics, att_new("mod", binary, true, NULL));
  aatt_add(statics, att_new("max", binary, true, NULL));
  aatt_add(statics, att_new("min", binary, true, NULL));

  Aatt *instance = class_instance(c);

  params = atype_new();
  atype_add(params, type_new_str());
  aatt_add(instance, att_new("toStr", type_new_fn(params), true, NULL));

  params = atype_new();
  atype_add(params, type_new_int());
  aatt_add(instance, att_new("toInt", type_new_fn(params), true, NULL));

  params = atype_new();
  atype_add(params, type_new_float());
  aatt_add(instance, att_new("toFloat", type_new_fn(params), true, NULL));

  arr_add(builtins, c);
}

