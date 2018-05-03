// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "builtin/tbool.h"

void tbool_add(Arr/*class*/ *builtins) {
  Atype *params;
  Type *tbool = type_new_bool();
  Type *tstr = type_new_str();
  Type *tp;
  Class *c = class_new("Bool");
  Aatt *statics = class_statics(c);

  params = atype_new();
  atype_add(params, tbool);
  tp = type_new_fn(params);
  aatt_add(statics, att_new(true, "rnd", tp, true, NULL));

  params = atype_new();
  atype_add(params, tbool);
  atype_add(params, tbool);
  tp = type_new_fn(params);
  aatt_add(statics, att_new(true, "not", tp, true, NULL));

  params = atype_new();
  atype_add(params, tbool);
  atype_add(params, tbool);
  atype_add(params, tbool);
  tp = type_new_fn(params);

  aatt_add(statics, att_new(true, "eq", tp, true, NULL));
  aatt_add(statics, att_new(true, "neq", tp, true, NULL));
  aatt_add(statics, att_new(true, "lt", tp, true, NULL));
  aatt_add(statics, att_new(true, "gt", tp, true, NULL));
  aatt_add(statics, att_new(true, "leq", tp, true, NULL));
  aatt_add(statics, att_new(true, "geq", tp, true, NULL));

  aatt_add(statics, att_new(true, "and", tp, true, NULL));
  aatt_add(statics, att_new(true, "or", tp, true, NULL));

  Aatt *instance = class_instance(c);
  params = atype_new();
  atype_add(params, tstr);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "toStr", tp, true, NULL));

  arr_add(builtins, c);
}

