// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_builtins.h"
#include "ast/Class.h"
#include "ast/Imported.h"

void tests_builtins() {
  puts("Builtins");

  size_t size = 0;

  Imported *im = imported_get();

  puts("--> Bool");
  Class *c = imported__class(im, "Bool");
  assert(c);
  char *cs = class_serialize(c);
  assert(!strcmp(cs, class_serialize(class_restore(cs))));
  Aatt *statics = class_statics(c);
  Aatt *instance = class_instance(c);

  void test_st(char *id, char *t) {
    Att *att = aatt__get(statics, id);
    if (!att) {
      printf("Attribute: %s\nExpected: %s\nActual: NULL\n", id, t);
      assert(false);
    }
    Type *tp = att_type(att);
    char *tps = type_to_str(tp);
    if (strcmp(tps, t)) {
      printf("Attribute: %s\nExpected: %s\nActual: %s\n", id, t, tps);
      assert(false);
    }
  }

  void test_ins(char *id, char *t) {
    Att *att = aatt__get(instance, id);
    if (!att) {
      printf("Attribute: %s\nExpected: %s\nActual: NULL\n", id, t);
      assert(false);
    }
    Type *tp = att_type(att);
    char *tps = type_to_str(tp);
    if (strcmp(tps, t)) {
      printf("Attribute: %s\nExpected: %s\nActual: %s\n", id, t, tps);
      assert(false);
    }
  }

  test_st("not", "(Bool:Bool)");
  test_st("rnd", "(:Bool)");

  test_st("eq", "(Bool,Bool:Bool)");
  test_st("neq", "(Bool,Bool:Bool)");
  test_st("lt", "(Bool,Bool:Bool)");
  test_st("gt", "(Bool,Bool:Bool)");
  test_st("leq", "(Bool,Bool:Bool)");
  test_st("geq", "(Bool,Bool:Bool)");

  test_st("and", "(Bool,Bool:Bool)");
  test_st("or", "(Bool,Bool:Bool)");

  test_ins("toStr", "(:Str)");

  size = arr_size(class_statics(c));
  size += arr_size(class_instance(c));
  assert(size == 11);

  puts("--> Byte");
  c = imported__class(im, "Byte");
  assert(c);
  cs = class_serialize(c);
  assert(!strcmp(cs, class_serialize(class_restore(cs))));
  statics = class_statics(c);
  instance = class_instance(c);

  test_st("bnot", "(Byte:Byte)");
  test_st("rinc", "(Byte:Byte)");
  test_st("rdec", "(Byte:Byte)");
  test_st("rnd", "(Byte:Byte)");
  test_st("linc", "(Byte:)");
  test_st("ldec", "(Byte:)");

  test_st("eq", "(Byte,Byte:Bool)");
  test_st("neq", "(Byte,Byte:Bool)");
  test_st("lt", "(Byte,Byte:Bool)");
  test_st("gt", "(Byte,Byte:Bool)");
  test_st("leq", "(Byte,Byte:Bool)");
  test_st("geq", "(Byte,Byte:Bool)");

  test_st("plus", "(Byte,Byte:Byte)");
  test_st("sub", "(Byte,Byte:Byte)");
  test_st("mul", "(Byte,Byte:Byte)");
  test_st("div", "(Byte,Byte:Byte)");
  test_st("mod", "(Byte,Byte:Byte)");
  test_st("max", "(Byte,Byte:Byte)");
  test_st("min", "(Byte,Byte:Byte)");
  test_st("and", "(Byte,Byte:Byte)");
  test_st("or", "(Byte,Byte:Byte)");
  test_st("xor", "(Byte,Byte:Byte)");
  test_st("lshift", "(Byte,Byte:Byte)");
  test_st("rshift", "(Byte,Byte:Byte)");
  test_st("rshift0", "(Byte,Byte:Byte)");

  test_ins("toStr", "(:Str)");
  test_ins("toChar", "(:Char)");
  test_ins("toInt", "(:Int)");
  test_ins("toFloat", "(:Float)");

  size = arr_size(class_statics(c));
  size += arr_size(class_instance(c));
  assert(size == 29);

  puts("--> Int");
  c = imported__class(im, "Int");
  assert(c);
  cs = class_serialize(c);
  assert(!strcmp(cs, class_serialize(class_restore(cs))));
  statics = class_statics(c);
  instance = class_instance(c);

  test_st("minus", "(Int:Int)");
  test_st("bnot", "(Int:Int)");
  test_st("rinc", "(Int:Int)");
  test_st("rdec", "(Int:Int)");
  test_st("rnd", "(Int:Int)");
  test_st("linc", "(Int:)");
  test_st("ldec", "(Int:)");

  test_st("eq", "(Int,Int:Bool)");
  test_st("neq", "(Int,Int:Bool)");
  test_st("lt", "(Int,Int:Bool)");
  test_st("gt", "(Int,Int:Bool)");
  test_st("leq", "(Int,Int:Bool)");
  test_st("geq", "(Int,Int:Bool)");

  test_st("plus", "(Int,Int:Int)");
  test_st("sub", "(Int,Int:Int)");
  test_st("mul", "(Int,Int:Int)");
  test_st("div", "(Int,Int:Int)");
  test_st("mod", "(Int,Int:Int)");
  test_st("pow", "(Int,Int:Int)");
  test_st("max", "(Int,Int:Int)");
  test_st("min", "(Int,Int:Int)");
  test_st("and", "(Int,Int:Int)");
  test_st("or", "(Int,Int:Int)");
  test_st("xor", "(Int,Int:Int)");
  test_st("lshift", "(Int,Int:Int)");
  test_st("rshift", "(Int,Int:Int)");
  test_st("rshift0", "(Int,Int:Int)");

  test_ins("toStr", "(:Str)");
  test_ins("toChar", "(:Char)");
  test_ins("toByte", "(:Byte)");
  test_ins("toFloat", "(:Float)");
  test_ins("abs", "(:Int)");

  size = arr_size(class_statics(c));
  size += arr_size(class_instance(c));
  assert(size == 32);

  puts("--> Float");
  c = imported__class(im, "Float");
  assert(c);
  cs = class_serialize(c);
  assert(!strcmp(cs, class_serialize(class_restore(cs))));
  statics = class_statics(c);
  instance = class_instance(c);

  test_st("e", "(:Float)");
  test_st("pi", "(:Float)");
  test_st("rnd", "(:Float)");

  test_st("minus", "(Float:Float)");

  test_st("eq", "(Float,Float:Bool)");
  test_st("neq", "(Float,Float:Bool)");
  test_st("lt", "(Float,Float:Bool)");
  test_st("gt", "(Float,Float:Bool)");
  test_st("leq", "(Float,Float:Bool)");
  test_st("geq", "(Float,Float:Bool)");

  test_st("plus", "(Float,Float:Float)");
  test_st("sub", "(Float,Float:Float)");
  test_st("mul", "(Float,Float:Float)");
  test_st("div", "(Float,Float:Float)");
  test_st("mod", "(Float,Float:Float)");
  test_st("atan2", "(Float,Float:Float)");
  test_st("pow", "(Float,Float:Float)");
  test_st("max", "(Float,Float:Float)");
  test_st("min", "(Float,Float:Float)");

  test_ins("toStr", "(:Str)");
  test_ins("toByte", "(:Byte)");
  test_ins("toInt", "(:Int)");
  test_ins("ceil", "(:Int)");
  test_ins("floor", "(:Int)");

  test_ins("abs", "(:Float)");
  test_ins("sin", "(:Float)");
  test_ins("cos", "(:Float)");
  test_ins("tan", "(:Float)");
  test_ins("asin", "(:Float)");
  test_ins("acos", "(:Float)");
  test_ins("atan", "(:Float)");
  test_ins("sinh", "(:Float)");
  test_ins("cosh", "(:Float)");
  test_ins("tanh", "(:Float)");
  test_ins("asinh", "(:Float)");
  test_ins("acosh", "(:Float)");
  test_ins("atanh", "(:Float)");
  test_ins("exp", "(:Float)");
  test_ins("exp2", "(:Float)");
  test_ins("exp10", "(:Float)");
  test_ins("log", "(:Float)");
  test_ins("log2", "(:Float)");
  test_ins("log10", "(:Float)");
  test_ins("sqrt", "(:Float)");
  test_ins("round", "(:Float)");

  size = arr_size(class_statics(c));
  size += arr_size(class_instance(c));
  assert(size == 45);

  puts("--> Char");
  c = imported__class(im, "Char");
  assert(c);
  cs = class_serialize(c);
  assert(!strcmp(cs, class_serialize(class_restore(cs))));
  statics = class_statics(c);
  instance = class_instance(c);

  test_st("eq", "(Char,Char:Bool)");
  test_st("neq", "(Char,Char:Bool)");
  test_st("lt", "(Char,Char:Bool)");
  test_st("gt", "(Char,Char:Bool)");
  test_st("leq", "(Char,Char:Bool)");
  test_st("geq", "(Char,Char:Bool)");

  test_ins("toStr", "(:Str)");
  test_ins("toBytes", "(:[Byte])");
  test_ins("toInt", "(:Int)");

  test_ins("isUpper", "(:Bool)");
  test_ins("isLower", "(:Bool)");
  test_ins("isLetter", "(:Bool)");
  test_ins("isDigit", "(:Bool)");
  test_ins("isLetterOrDigit", "(:Bool)");

  test_ins("upper", "(:Char)");
  test_ins("lower", "(:Char)");

  test_ins("repeat", "(Int:Str)");

  size = arr_size(class_statics(c));
  size += arr_size(class_instance(c));
  assert(size == 17);

  puts("--> Str");
  c = imported__class(im, "Str");
  assert(c);
  cs = class_serialize(c);
  assert(!strcmp(cs, class_serialize(class_restore(cs))));
  statics = class_statics(c);
  instance = class_instance(c);

  test_st("eq", "(Str,Str:Bool)");
  test_st("neq", "(Str,Str:Bool)");
  test_st("lt", "(Str,Str:Bool)");
  test_st("gt", "(Str,Str:Bool)");
  test_st("leq", "(Str,Str:Bool)");
  test_st("geq", "(Str,Str:Bool)");

  test_st("join", "([Str],Str:Str)");

  test_ins("toStr", "(:Str)");
  test_ins("lower", "(:Str)");
  test_ins("upper", "(:Str)");
  test_ins("trim", "(:Str)");
  test_ins("ltrim", "(:Str)");
  test_ins("rtrim", "(:Str)");

  test_ins("len", "(:Int)");

  test_ins("toBytes", "(:[Byte])");

  test_ins("contains", "(Str:Bool)");
  test_ins("starts", "(Str:Bool)");
  test_ins("ends", "(Str:Bool)");

  test_ins("index", "(Str:Int)");
  test_ins("lastIndex", "(Str:Int)");

  test_ins("get", "(Int:Char)");

  test_ins("repeat", "(Int:Str)");
  test_ins("sub0", "(Int:Str)");
  test_ins("subEnd", "(Int:Str)");

  test_ins("split", "(Str:[Str])");

  test_ins("sub", "(Int,Int:Str)");

  test_ins("replace", "(Str,Str:Str)");

  size = arr_size(class_statics(c));
  size += arr_size(class_instance(c));
  assert(size == 27);

  puts("--> Arr");
  c = imported__class(im, "Arr");
  assert(c);
  cs = class_serialize(c);
  assert(!strcmp(cs, class_serialize(class_restore(cs))));
  statics = class_statics(c);
  instance = class_instance(c);

  test_st("eq", "([A],[A]:Bool)");
  test_st("neq", "([A],[A]:Bool)");
  test_st("lt", "([A],[A]:Bool)");
  test_st("gt", "([A],[A]:Bool)");
  test_st("leq", "([A],[A]:Bool)");
  test_st("geq", "([A],[A]:Bool)");

  test_st("plus", "([A],[A]:[A])");

  test_ins("toStr", "(:Str)");
  test_ins("size", "(:Int)");

  test_ins("shuffle", "(:[A])");
  test_ins("reverse", "(:[A])");

  test_ins("pop", "(:A)");
  test_ins("pop0", "(:A)");
  test_ins("peek", "(:A)");
  test_ins("peek0", "(:A)");

  test_ins("push", "(A:[A])");
  test_ins("push0", "(A:[A])");

  test_ins("get", "(Int:A)");
  test_ins("remove", "(Int:A)");

  test_ins("sub0", "(Int:[A])");
  test_ins("subEnd", "(Int:[A])");
  test_ins("remove0", "(Int:[A])");
  test_ins("removeEnd", "(Int:[A])");

  test_ins("sub", "(Int,Int:[A])");
  test_ins("removeRange", "(Int,Int:[A])");

  test_ins("set", "(Int,A:[A])");

  size = arr_size(class_statics(c));
  size += arr_size(class_instance(c));
  assert(size == 26);

  puts("--> Map");
  c = imported__class(im, "Map");
  assert(c);
  cs = class_serialize(c);
  assert(!strcmp(cs, class_serialize(class_restore(cs))));
  statics = class_statics(c);
  instance = class_instance(c);

  test_st("eq", "({A},{A}:Bool)");
  test_st("neq", "({A},{A}:Bool)");

  test_st("plus", "({A},{A}:{A})");

  test_ins("toStr", "(:Str)");
  test_ins("keys", "(:[Str])");
  test_ins("values", "(:[A])");
  test_ins("keysValues", "(:[[*]])");
  test_ins("size", "(:Int)");

  test_ins("get", "(Str:A)");
  test_ins("remove", "(Str:A)");

  test_ins("set", "(Str,A:{A})");

  size = arr_size(class_statics(c));
  size += arr_size(class_instance(c));
  assert(size == 11);

  puts("    Finished");
}
