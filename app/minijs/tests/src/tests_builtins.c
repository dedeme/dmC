// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_builtins.h"
#include "ast/Class.h"
#include "ast/Imported.h"

void tests_builtins() {
  puts("Builtins");

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
  test_st("eq", "(Bool,Bool:Bool)");
  test_st("neq", "(Bool,Bool:Bool)");
  test_st("lt", "(Bool,Bool:Bool)");
  test_st("gt", "(Bool,Bool:Bool)");
  test_st("leq", "(Bool,Bool:Bool)");
  test_st("geq", "(Bool,Bool:Bool)");

  test_st("and", "(Bool,Bool:Bool)");
  test_st("or", "(Bool,Bool:Bool)");

  test_ins("toStr", "(:Str)");


  puts("--> Byte");
  c = imported__class(im, "Byte");
  assert(c);
  cs = class_serialize(c);
  assert(!strcmp(cs, class_serialize(class_restore(cs))));
  statics = class_statics(c);
  instance = class_instance(c);

  test_st("minus", "(Byte:Byte)");
  test_st("rinc", "(Byte:Byte)");
  test_st("rdec", "(Byte:Byte)");
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

  test_ins("toStr", "(:Str)");
  test_ins("toInt", "(:Int)");
  test_ins("toFloat", "(:Float)");

  puts("    Finished");
}
