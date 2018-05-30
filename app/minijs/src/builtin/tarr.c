// Copyright 1-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "builtin/tarr.h"

void tarr_add(Arr/*class*/ *builtins) {
  Atype *params;
  Type *tp;
//  Type *tvoid = type_new_void();
  Type *tbool = type_new_bool();
  Type *tint = type_new_int();
  Type *tstr = type_new_str();
  Type *tdata = type_new_data("A", arr_new());
  Type *tarr = type_new_arr(tdata);
  Class *c = class_new("Arr");
  achar_add(class_generics(c), "A");
  Aatt *statics = class_statics(c);

  params = atype_new();
  atype_add(params, tarr);
  atype_add(params, tarr);
  atype_add(params, tbool);
  tp = type_new_fn(params);

  aatt_add(statics, att_new(true, "eq", tp, true, NULL));
  aatt_add(statics, att_new(true, "neq", tp, true, NULL));
  aatt_add(statics, att_new(true, "lt", tp, true, NULL));
  aatt_add(statics, att_new(true, "gt", tp, true, NULL));
  aatt_add(statics, att_new(true, "leq", tp, true, NULL));
  aatt_add(statics, att_new(true, "geq", tp, true, NULL));

  params = atype_new();
  atype_add(params, tarr);
  atype_add(params, tarr);
  atype_add(params, tarr);
  tp = type_new_fn(params);
  aatt_add(statics, att_new(true, "plus", tp, true, NULL));

  Aatt *instance = class_instance(c);

  params = atype_new();
  atype_add(params, tstr);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "toStr", tp, true, NULL));

  params = atype_new();
  atype_add(params, tint);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "size", tp, true, NULL));

  params = atype_new();
  atype_add(params, tarr);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "shuffle", tp, true, NULL));
  aatt_add(instance, att_new(true, "reverse", tp, true, NULL));

  params = atype_new();
  atype_add(params, tdata);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "pop", tp, true, NULL));
  aatt_add(instance, att_new(true, "pop0", tp, true, NULL));
  aatt_add(instance, att_new(true, "peek", tp, true, NULL));
  aatt_add(instance, att_new(true, "peek0", tp, true, NULL));

  params = atype_new();
  atype_add(params, tdata);
  atype_add(params, tarr);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "push", tp, true, NULL));
  aatt_add(instance, att_new(true, "push0", tp, true, NULL));

  params = atype_new();
  atype_add(params, tint);
  atype_add(params, tdata);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "get", tp, true, NULL));
  aatt_add(instance, att_new(true, "remove", tp, true, NULL));

  params = atype_new();
  atype_add(params, tint);
  atype_add(params, tarr);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "sub0", tp, true, NULL));
  aatt_add(instance, att_new(true, "subEnd", tp, true, NULL));
  aatt_add(instance, att_new(true, "remove0", tp, true, NULL));
  aatt_add(instance, att_new(true, "removeEnd", tp, true, NULL));

  params = atype_new();
  atype_add(params, tint);
  atype_add(params, tint);
  atype_add(params, tarr);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "sub", tp, true, NULL));
  aatt_add(instance, att_new(true, "removeRange", tp, true, NULL));

  params = atype_new();
  atype_add(params, tint);
  atype_add(params, tdata);
  atype_add(params, tarr);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "set", tp, true, NULL));

  arr_add(builtins, c);
}

