// Copyright 1-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "builtin/tstr.h"

void tstr_add(Arr/*class*/ *builtins) {
  Atype *params;
  Type *tp;
  Type *tbool = type_new_bool();
  Type *tbyte = type_new_byte();
  Type *tint = type_new_int();
  Type *tchar = type_new_char();
  Type *tstr = type_new_str();
  Class *c = class_new("Str");
  Aatt *statics = class_statics(c);

  params = atype_new();
  atype_add(params, tstr);
  atype_add(params, tstr);
  atype_add(params, tbool);
  tp = type_new_fn(params);

  aatt_add(statics, att_new(true, "eq", tp, true, NULL));
  aatt_add(statics, att_new(true, "neq", tp, true, NULL));
  aatt_add(statics, att_new(true, "lt", tp, true, NULL));
  aatt_add(statics, att_new(true, "gt", tp, true, NULL));
  aatt_add(statics, att_new(true, "leq", tp, true, NULL));
  aatt_add(statics, att_new(true, "geq", tp, true, NULL));

  params = atype_new();
  atype_add(params, type_new_arr(tstr));
  atype_add(params, tstr);
  atype_add(params, tstr);
  tp = type_new_fn(params);

  aatt_add(statics, att_new(true, "join", tp, true, NULL));

  Aatt *instance = class_instance(c);

  params = atype_new();
  atype_add(params, tstr);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "toStr", tp, true, NULL));
  aatt_add(instance, att_new(true, "lower", tp, true, NULL));
  aatt_add(instance, att_new(true, "upper", tp, true, NULL));
  aatt_add(instance, att_new(true, "trim", tp, true, NULL));
  aatt_add(instance, att_new(true, "ltrim", tp, true, NULL));
  aatt_add(instance, att_new(true, "rtrim", tp, true, NULL));

  params = atype_new();
  atype_add(params, tint);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "len", tp, true, NULL));

  params = atype_new();
  atype_add(params, type_new_arr(tbyte));
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "toBytes", tp, true, NULL));

  params = atype_new();
  atype_add(params, tstr);
  atype_add(params, tbool);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "contains", tp, true, NULL));
  aatt_add(instance, att_new(true, "starts", tp, true, NULL));
  aatt_add(instance, att_new(true, "ends", tp, true, NULL));

  params = atype_new();
  atype_add(params, tstr);
  atype_add(params, tint);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "index", tp, true, NULL));
  aatt_add(instance, att_new(true, "lastIndex", tp, true, NULL));

  params = atype_new();
  atype_add(params, tint);
  atype_add(params, tchar);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "get", tp, true, NULL));

  params = atype_new();
  atype_add(params, tint);
  atype_add(params, tstr);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "repeat", tp, true, NULL));
  aatt_add(instance, att_new(true, "sub0", tp, true, NULL));
  aatt_add(instance, att_new(true, "subEnd", tp, true, NULL));

  params = atype_new();
  atype_add(params, tstr);
  atype_add(params, type_new_arr(tstr));
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "split", tp, true, NULL));

  params = atype_new();
  atype_add(params, tint);
  atype_add(params, tint);
  atype_add(params, tstr);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "sub", tp, true, NULL));

  params = atype_new();
  atype_add(params, tstr);
  atype_add(params, tstr);
  atype_add(params, tstr);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "replace", tp, true, NULL));

  arr_add(builtins, c);
}

