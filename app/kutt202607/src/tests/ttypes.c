// Copyright 12-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "tests/ttypes.h"
#include "data/type.h"

static Type *read(char *s) {
  Rs *rs = type_read(s);
  Type *t = rs_get(rs);
  if (!t) EXC_KUTT(rs_error(rs));
  return t;
}

static char *readE(char *s) {
  Rs *rs = type_read(s);
  return rs_error(rs);
}

static char *to_str (Type *t) {
  return type_to_str(t);
}

static void test (Type *actual, Type *expected) {
  if (strcmp(to_str(actual),to_str(expected)))
    EXC_KUTT(str_f(
      "Test failed:\nExpected: %s\n  Actual: %s",
      to_str(expected), to_str(actual)
    ));
}

static void testE (char *actual, char *expected) {
  if (strcmp(actual, expected))
    EXC_KUTT(str_f(
      "Test failed:\nExpected: %s\n  Actual: %s",
      expected, actual
    ));
}

static void test_str (char *s) {
  Type *t = read(s);
  Type *t2 = read(to_str(read(s)));
  test(t2, t);
}

void ttypes_run() {
  puts("types:");

  Arr *a0 = arr_new();

  test(read("b"), type_bool());
  test(read("i"), type_int());
  test(read("f"), type_float());
  test(read("s"), type_string());

  test(read("  A   "), type_new("A", a0));
  test(read("Z"), type_new("Z", a0));

  test(read(" < file > "), type_new("<file>", a0));

  test(read(" ( abc ) ") , type_new("(abc)", a0));
  test(read(" ( z0.abc ) ") , type_new("(z0.abc)", a0));

  test(read(" [. s ] "), type_new("t", arr_new_from(type_string(), NULL)));
  test(
    read(" [. s<file> ( a) ] "),
    type_new("t", arr_new_from(
      type_string(), type_new("<file>", a0), type_new("(a)", a0), NULL
    ))
  );
  test(
    read(" [. i [.s<file> ( a)] ] "),
    type_new("t", arr_new_from(
      type_int(),
      type_new("t", arr_new_from(
        type_string(), type_new("<file>", a0), type_new("(a)", a0), NULL
      )),
      NULL
    ))
  );

  test(read("[ | ] "), type_new("0", a0));
  test(read("[ | ( ab ) ] "), type_new("1", arr_new_from(
    type_new("(ab)", a0), NULL
  )));
  test(
    read("[ i | b ] "), type_new("1", arr_new_from(
      type_int(), type_bool(), NULL
    ))
  );
  test(
    read(" [ i [.s<file> ( a)] | [.s ] ]"),
    type_new("1", arr_new_from(
      type_int(),
      type_new("t", arr_new_from(
        type_string(), type_new("<file>", a0), type_new("(a)", a0), NULL
      )),
      type_new("t", arr_new_from(type_string(), NULL)),
      NULL
    ))
  );

  test(read("[ b ]"), type_new("a", arr_new_from(type_bool(), NULL)));
  test(read("[! [i] ]"), type_new("r", arr_new_from(
    type_new("a", arr_new_from(type_int(), NULL)), NULL
  )));

  test(read("{ [i] }"), type_new("d", arr_new_from(
    type_new("a", arr_new_from(type_int(), NULL)), NULL
  )));
  test(read("{ [  A] }"), type_new("d", arr_new_from(
    type_new("a", arr_new_from(type_new("A",a0), NULL)), NULL
  )));

  TEST(to_str(read(" b ")), "b");
  TEST(to_str(read(" i ")), "i");
  TEST(to_str(read(" f ")), "f");
  TEST(to_str(read(" s ")), "s");
  TEST(to_str(read(" A ")), "A");
  TEST(to_str(read("Z")), "Z");
  TEST(to_str(read(" < abc > ")), "<abc>");
  TEST(to_str(read(" ( abc ) ")), "(abc)");
  TEST(to_str(read(" ( abc | AB ) ")), "(abc|AB)");
  TEST(to_str(read(" ( abc | A i ) ")), "(abc|Ai)");
  TEST(to_str(read(" [( abc | s  [. (ef)(g| A )] ) ]")), "[(abc|s[.(ef)(g|A)])]");
  TEST(to_str(read(" [ . s ] ")), "[.s]");
  TEST(to_str(read(" [. <file> i s ] ")), "[.<file>is]");
  TEST(to_str(read(" [ <file> i s | ] ")), "[<file>is|]");
  TEST(to_str(read(" [ <file> i s | (  abc ) ] ")), "[<file>is|(abc)]");
  TEST(to_str(read(" [ <file> A Z | (  abc ) ] ")), "[<file>AZ|(abc)]");

  test_str("b");
  test_str(" i ");
  test_str("f");
  test_str("  s   ");
  test_str("  A   ");
  test_str("Z");
  test_str(" < file > ");
  test_str(" ( abc ) ");
  test_str(" ( z0 . abc ) ");
  test_str(" [ s ] ");
  test_str(" [. s<file> ( a) ] ");
  test_str(" [. i [.s<file> ( a)] ] ");
  test_str("[ | ] ");
  test_str("[ | ( ab ) ] ");
  test_str("[ i | b ] ");
  test_str(" [ i [.s<file> ( a)] | [s ] ]");
  test_str("[. b ]");
  test_str("[! [.i] ]");
  test_str("{ [.i] }");

  // ERRORS ----------------------------

  testE(readE("b i "), "Extra charcters ' i ' in 'b i '");
  testE(readE("A i "), "Extra charcters ' i ' in 'A i '");

  testE(readE(" < file  "), "'>' is missing in ' < file  '");
  testE(
    readE(" < 0file > "),
    "'0file' is not a valid identifier in ' < 0file > '"
  );
  testE(
    readE(" < fi-le > "),
    "'fi-le' is not a valid identifier in ' < fi-le > '"
  );

  testE(
    readE(" ( abc  "),
    "')' is missing in ' ( abc  '"
  );
  testE(
    readE(" ( 0abc ) "),
    "'0abc' is not a valid identifier in ' ( 0abc ) '"
  );
  testE(
    readE(" ( a_b ) "),
    "'a_b' is not a valid identifier in ' ( a_b ) '"
  );
  testE(
    readE(" ( 0a . b ) "),
    "'0a' is not a valid identifier in ' ( 0a . b ) '"
  );
  testE(
    readE(" ( a . !b ) "),
    "'!b' is not a valid identifier in ' ( a . !b ) '"
  );

  testE(
    readE(" [ x ] "),
    "Unexpected 'x' in ' [ x ] '"
  );
  testE(
    readE(" [ Zi  "),
    "']' is missing in ' [ Zi  '"
  );
  testE(
    readE(" [ ]"),
    "'[]' is not a valid type in ' [ ]'"
  );
  testE(
    readE(" [. ]"),
    "Unexpected ']' in ' [. ]'"
  );
  testE(
    readE(" [! ]"),
    "Unexpected ']' in ' [! ]'"
  );
  testE(
    readE(" [ i i]"),
    "Array with more than one subtype in ' [ i i]'"
  );
  testE(
    readE(" [! i i]"),
    "Expected ']', but fund 'i]' in ' [! i i]'"
  );
  testE(
    readE(" [|i"),
    "']' is missing in ' [|i'"
  );
  testE(
    readE(" [|is]"),
    "Expected ']', but fund 's]' in ' [|is]'"
  );
  testE(
    readE("[||]"),
    "Unexpected '|' in '[||]'"
  );

  testE(
    readE(" { }"),
    "'{}' is not a valid type in ' { }'"
  );
  testE(
    readE(" { i "),
    "'}' is missing in ' { i '"
  );
  testE(
    readE(" { i s "),
    "Expected '}', but fund 's' in ' { i s '"
  );
  testE(
    readE(" (  .abc ) ") ,
    "Identifier start with dot in ' (  .abc ) '"
  );
  testE(
    readE(" ( abc | ) "),
    "Definition parameters are missing in ' ( abc | ) '"
  );

  puts("  finished");
}

