// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "builtin/tbool.h"

void tbool_add(Arr/*class*/ *builtins) {
  Class *c = class_new("Bool");
  Aatt *statics = class_statics(c);

  Atype *params = atype_new();
  atype_add(params, type_new_bool());
  atype_add(params, type_new_bool());
  Type *unary = type_new_fn(params);
  aatt_add(statics, att_new("not", unary, true, NULL));
  aatt_add(statics, att_new("rnd", unary, true, NULL));


  params = atype_new();
  atype_add(params, type_new_bool());
  atype_add(params, type_new_bool());
  atype_add(params, type_new_bool());
  Type *binary = type_new_fn(params);

  aatt_add(statics, att_new("eq", binary, true, NULL));
  aatt_add(statics, att_new("neq", binary, true, NULL));
  aatt_add(statics, att_new("lt", binary, true, NULL));
  aatt_add(statics, att_new("gt", binary, true, NULL));
  aatt_add(statics, att_new("leq", binary, true, NULL));
  aatt_add(statics, att_new("geq", binary, true, NULL));

  aatt_add(statics, att_new("and", binary, true, NULL));
  aatt_add(statics, att_new("or", binary, true, NULL));

  Aatt *instance = class_instance(c);
  params = atype_new();
  atype_add(params, type_new_str());
  aatt_add(instance, att_new("toStr", type_new_fn(params), true, NULL));

  arr_add(builtins, c);
}

