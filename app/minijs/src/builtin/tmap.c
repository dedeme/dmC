// Copyright 1-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "builtin/tmap.h"

void tmap_add(Arr/*class*/ *builtins) {
  Atype *params;
  Type *tp;
  Type *tbool = type_new_bool();
  Type *tint = type_new_int();
  Type *tstr = type_new_str();
  Type *tdata = type_new_data("A", arr_new());
  Type *tmap = type_new_map(tdata);
  Class *c = class_new("Map");
  achar_add(class_generics(c), "A");
  Aatt *statics = class_statics(c);

  params = atype_new();
  atype_add(params, tmap);
  atype_add(params, tmap);
  atype_add(params, tbool);
  tp = type_new_fn(params);

  aatt_add(statics, att_new(true, "eq", tp, true, NULL));
  aatt_add(statics, att_new(true, "neq", tp, true, NULL));

  params = atype_new();
  atype_add(params, tmap);
  atype_add(params, tmap);
  atype_add(params, tmap);
  tp = type_new_fn(params);
  aatt_add(statics, att_new(true, "plus", tp, true, NULL));

  Aatt *instance = class_instance(c);

  params = atype_new();
  atype_add(params, tstr);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "toStr", tp, true, NULL));

  params = atype_new();
  atype_add(params, type_new_arr(tstr));
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "keys", tp, true, NULL));

  params = atype_new();
  atype_add(params, type_new_arr(tdata));
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "values", tp, true, NULL));

  params = atype_new();
  atype_add(params, type_new_arr(type_new_arr(type_new_any())));
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "keysValues", tp, true, NULL));

  params = atype_new();
  atype_add(params, tint);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "size", tp, true, NULL));

  params = atype_new();
  atype_add(params, tstr);
  atype_add(params, tdata);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "get", tp, true, NULL));
  aatt_add(instance, att_new(true, "remove", tp, true, NULL));

  params = atype_new();
  atype_add(params, tstr);
  atype_add(params, tdata);
  atype_add(params, tmap);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "set", tp, true, NULL));

  arr_add(builtins, c);
}

