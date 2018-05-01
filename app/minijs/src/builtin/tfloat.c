// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "builtin/tfloat.h"

void tfloat_add(Arr/*class*/ *builtins) {
  Class *c = class_new("Float");
  Aatt *statics = class_statics(c);

  Atype *params = atype_new();
  atype_add(params, type_new_float());
  atype_add(params, type_new_float());
  Type *unary = type_new_fn(params);
  aatt_add(statics, att_new("minus", unary, true, NULL));
  aatt_add(statics, att_new("abs", unary, true, NULL));
  aatt_add(statics, att_new("sin", unary, true, NULL));
  aatt_add(statics, att_new("cos", unary, true, NULL));
  aatt_add(statics, att_new("tan", unary, true, NULL));
  aatt_add(statics, att_new("asin", unary, true, NULL));
  aatt_add(statics, att_new("acos", unary, true, NULL));
  aatt_add(statics, att_new("atan", unary, true, NULL));
  aatt_add(statics, att_new("sinh", unary, true, NULL));
  aatt_add(statics, att_new("cosh", unary, true, NULL));
  aatt_add(statics, att_new("tanh", unary, true, NULL));
  aatt_add(statics, att_new("asinh", unary, true, NULL));
  aatt_add(statics, att_new("acosh", unary, true, NULL));
  aatt_add(statics, att_new("atanh", unary, true, NULL));
  aatt_add(statics, att_new("exp", unary, true, NULL));
  aatt_add(statics, att_new("exp2", unary, true, NULL));
  aatt_add(statics, att_new("exp10", unary, true, NULL));
  aatt_add(statics, att_new("log", unary, true, NULL));
  aatt_add(statics, att_new("log2", unary, true, NULL));
  aatt_add(statics, att_new("log10", unary, true, NULL));
  aatt_add(statics, att_new("sqrt", unary, true, NULL));


  params = atype_new();
  atype_add(params, type_new_float());
  unary = type_new_fn(params);
  aatt_add(statics, att_new("e", unary, true, NULL));
  aatt_add(statics, att_new("pi", unary, true, NULL));
  aatt_add(statics, att_new("rnd", unary, true, NULL));

  params = atype_new();
  atype_add(params, type_new_float());
  atype_add(params, type_new_float());
  atype_add(params, type_new_bool());
  Type *binary = type_new_fn(params);

  aatt_add(statics, att_new("eq", binary, true, NULL));
  aatt_add(statics, att_new("neq", binary, true, NULL));
  aatt_add(statics, att_new("lt", binary, true, NULL));
  aatt_add(statics, att_new("gt", binary, true, NULL));
  aatt_add(statics, att_new("leq", binary, true, NULL));
  aatt_add(statics, att_new("geq", binary, true, NULL));

  params = atype_new();
  atype_add(params, type_new_float());
  atype_add(params, type_new_float());
  atype_add(params, type_new_float());
  binary = type_new_fn(params);

  aatt_add(statics, att_new("plus", binary, true, NULL));
  aatt_add(statics, att_new("sub", binary, true, NULL));
  aatt_add(statics, att_new("mul", binary, true, NULL));
  aatt_add(statics, att_new("div", binary, true, NULL));
  aatt_add(statics, att_new("mod", binary, true, NULL));
  aatt_add(statics, att_new("atan2", binary, true, NULL));
  aatt_add(statics, att_new("pow", binary, true, NULL));
  aatt_add(statics, att_new("max", binary, true, NULL));
  aatt_add(statics, att_new("min", binary, true, NULL));

  Aatt *instance = class_instance(c);

  params = atype_new();
  atype_add(params, type_new_str());
  aatt_add(instance, att_new("toStr", type_new_fn(params), true, NULL));

  params = atype_new();
  atype_add(params, type_new_int());
  aatt_add(instance, att_new("toInt", type_new_fn(params), true, NULL));
  aatt_add(instance, att_new("round", type_new_fn(params), true, NULL));
  aatt_add(instance, att_new("ceil", type_new_fn(params), true, NULL));
  aatt_add(instance, att_new("floor", type_new_fn(params), true, NULL));

  params = atype_new();
  atype_add(params, type_new_byte());
  aatt_add(instance, att_new("toByte", type_new_fn(params), true, NULL));

  arr_add(builtins, c);
}

