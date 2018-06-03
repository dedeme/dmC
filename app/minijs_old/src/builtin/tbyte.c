// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "builtin/tbyte.h"

void tbyte_add(Arr/*class*/ *builtins) {
  Atype *params;
  Type *tp;
  Type *tbool = type_new_bool();
  Type *tbyte = type_new_byte();
  Type *tint = type_new_int();
  Type *tchar = type_new_char();
  Type *tstr = type_new_str();
  Class *c = class_new("Byte");
  Aatt *statics = class_statics(c);

  params = atype_new();
  atype_add(params, tbyte);
  atype_add(params, tbyte);
  tp = type_new_fn(params);
  aatt_add(statics, att_new(true, "bnot", tp, true, NULL));
  aatt_add(statics, att_new(true, "rinc", tp, true, NULL));
  aatt_add(statics, att_new(true, "rdec", tp, true, NULL));
  aatt_add(statics, att_new(true, "rnd", tp, true, NULL));

  params = atype_new();
  atype_add(params, tbyte);
  atype_add(params, type_new_void());
  tp = type_new_fn(params);
  aatt_add(statics, att_new(true, "linc", tp, true, NULL));
  aatt_add(statics, att_new(true, "ldec", tp, true, NULL));

  params = atype_new();
  atype_add(params, tbyte);
  atype_add(params, tbyte);
  atype_add(params, tbool);
  tp = type_new_fn(params);

  aatt_add(statics, att_new(true, "eq", tp, true, NULL));
  aatt_add(statics, att_new(true, "neq", tp, true, NULL));
  aatt_add(statics, att_new(true, "lt", tp, true, NULL));
  aatt_add(statics, att_new(true, "gt", tp, true, NULL));
  aatt_add(statics, att_new(true, "leq", tp, true, NULL));
  aatt_add(statics, att_new(true, "geq", tp, true, NULL));

  params = atype_new();
  atype_add(params, tbyte);
  atype_add(params, tbyte);
  atype_add(params, tbyte);
  tp = type_new_fn(params);

  aatt_add(statics, att_new(true, "plus", tp, true, NULL));
  aatt_add(statics, att_new(true, "sub", tp, true, NULL));
  aatt_add(statics, att_new(true, "mul", tp, true, NULL));
  aatt_add(statics, att_new(true, "div", tp, true, NULL));
  aatt_add(statics, att_new(true, "mod", tp, true, NULL));
  aatt_add(statics, att_new(true, "max", tp, true, NULL));
  aatt_add(statics, att_new(true, "min", tp, true, NULL));
  aatt_add(statics, att_new(true, "and", tp, true, NULL));
  aatt_add(statics, att_new(true, "or", tp, true, NULL));
  aatt_add(statics, att_new(true, "xor", tp, true, NULL));
  aatt_add(statics, att_new(true, "lshift", tp, true, NULL));
  aatt_add(statics, att_new(true, "rshift", tp, true, NULL));
  aatt_add(statics, att_new(true, "rshift0", tp, true, NULL));

  Aatt *instance = class_instance(c);

  params = atype_new();
  atype_add(params, tstr);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "toStr", tp, true, NULL));

  params = atype_new();
  atype_add(params, tchar);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "toChar", tp, true, NULL));

  params = atype_new();
  atype_add(params, tint);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "toInt", tp, true, NULL));

  params = atype_new();
  atype_add(params, type_new_float());
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "toFloat", tp, true, NULL));

  arr_add(builtins, c);
}

