// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "builtin/tfloat.h"

void tfloat_add(Arr/*class*/ *builtins) {
  Atype *params;
  Type *tp;
  Type *tbool = type_new_bool();
  Type *tbyte = type_new_byte();
  Type *tint = type_new_int();
  Type *tfloat = type_new_float();
  Type *tstr = type_new_str();
  Class *c = class_new("Float");
  Aatt *statics = class_statics(c);

  params = atype_new();
  atype_add(params, tfloat);
  atype_add(params, tfloat);
  tp = type_new_fn(params);
  aatt_add(statics, att_new(true, "minus", tp, true, NULL));

  params = atype_new();
  atype_add(params, tfloat);
  tp = type_new_fn(params);
  aatt_add(statics, att_new(true, "e", tp, true, NULL));
  aatt_add(statics, att_new(true, "pi", tp, true, NULL));
  aatt_add(statics, att_new(true, "rnd", tp, true, NULL));

  params = atype_new();
  atype_add(params, tfloat);
  atype_add(params, tfloat);
  atype_add(params, tbool);
  tp = type_new_fn(params);

  aatt_add(statics, att_new(true, "eq", tp, true, NULL));
  aatt_add(statics, att_new(true, "neq", tp, true, NULL));
  aatt_add(statics, att_new(true, "lt", tp, true, NULL));
  aatt_add(statics, att_new(true, "gt", tp, true, NULL));
  aatt_add(statics, att_new(true, "leq", tp, true, NULL));
  aatt_add(statics, att_new(true, "geq", tp, true, NULL));

  params = atype_new();
  atype_add(params, tfloat);
  atype_add(params, tfloat);
  atype_add(params, tfloat);
  tp = type_new_fn(params);

  aatt_add(statics, att_new(true, "plus", tp, true, NULL));
  aatt_add(statics, att_new(true, "sub", tp, true, NULL));
  aatt_add(statics, att_new(true, "mul", tp, true, NULL));
  aatt_add(statics, att_new(true, "div", tp, true, NULL));
  aatt_add(statics, att_new(true, "mod", tp, true, NULL));
  aatt_add(statics, att_new(true, "atan2", tp, true, NULL));
  aatt_add(statics, att_new(true, "pow", tp, true, NULL));
  aatt_add(statics, att_new(true, "max", tp, true, NULL));
  aatt_add(statics, att_new(true, "min", tp, true, NULL));

  Aatt *instance = class_instance(c);

  params = atype_new();
  atype_add(params, tstr);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "toStr", tp, true, NULL));

  params = atype_new();
  atype_add(params, tint);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "toInt", tp, true, NULL));
  aatt_add(instance, att_new(true, "ceil", tp, true, NULL));
  aatt_add(instance, att_new(true, "floor", tp, true, NULL));

  params = atype_new();
  atype_add(params, tbyte);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "toByte", tp, true, NULL));

  params = atype_new();
  atype_add(params, tfloat);
  tp = type_new_fn(params);
  aatt_add(instance, att_new(true, "abs", tp, true, NULL));
  aatt_add(instance, att_new(true, "sin", tp, true, NULL));
  aatt_add(instance, att_new(true, "cos", tp, true, NULL));
  aatt_add(instance, att_new(true, "tan", tp, true, NULL));
  aatt_add(instance, att_new(true, "asin", tp, true, NULL));
  aatt_add(instance, att_new(true, "acos", tp, true, NULL));
  aatt_add(instance, att_new(true, "atan", tp, true, NULL));
  aatt_add(instance, att_new(true, "sinh", tp, true, NULL));
  aatt_add(instance, att_new(true, "cosh", tp, true, NULL));
  aatt_add(instance, att_new(true, "tanh", tp, true, NULL));
  aatt_add(instance, att_new(true, "asinh", tp, true, NULL));
  aatt_add(instance, att_new(true, "acosh", tp, true, NULL));
  aatt_add(instance, att_new(true, "atanh", tp, true, NULL));
  aatt_add(instance, att_new(true, "exp", tp, true, NULL));
  aatt_add(instance, att_new(true, "exp2", tp, true, NULL));
  aatt_add(instance, att_new(true, "exp10", tp, true, NULL));
  aatt_add(instance, att_new(true, "log", tp, true, NULL));
  aatt_add(instance, att_new(true, "log2", tp, true, NULL));
  aatt_add(instance, att_new(true, "log10", tp, true, NULL));
  aatt_add(instance, att_new(true, "sqrt", tp, true, NULL));
  aatt_add(instance, att_new(true, "round", tp, true, NULL));

  arr_add(builtins, c);
}

