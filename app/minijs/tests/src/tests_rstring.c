// Copyright 5-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_rstring.h"
#include "ast/Value.h"
#include "lexer/rstring.h"

static Tx *mk_tx(char *t) {
  return tx_new("Empty", t, t, 1, 0);
}

static void check_char(Value *v, char *s) {
  assert(value_vtype(v) == VCHAR);
  assert(type_type(value_type(v)) == DATA);
  assert(!strcmp(type_id(value_type(v)), "Char"));
  assert(arr_size(value_attachs(v)) == 0);

  if (strcmp(achar_get(value_data(v), 0), s)) {
    printf("Expected: %s\nActual:%s\n", s, achar_get(value_data(v), 0));
    assert(false);
  }
}

static void check_str(Value *v, char *s) {
  assert(value_vtype(v) == VSTR);
  assert(type_type(value_type(v)) == DATA);
  assert(!strcmp(type_id(value_type(v)), "Str"));
  assert(arr_size(value_attachs(v)) == 0);

  if (strcmp(achar_get(value_data(v), 0), s)) {
    printf("Expected: %s\nActual:%s\n", s, achar_get(value_data(v), 0));
    assert(false);
  }
}

static void check_str2(Value *v, char *s) {
  assert(value_vtype(v) == VSTR2);
  assert(type_type(value_type(v)) == DATA);
  assert(!strcmp(type_id(value_type(v)), "Str"));
  assert(arr_size(value_attachs(v)) == 0);

  if (strcmp(achar_get(value_data(v), 0), s)) {
    printf("Expected: %s\nActual:%s\n", s, achar_get(value_data(v), 0));
    assert(false);
  }
}

void tests_rstring() {
  puts("Reader: string");
  Tx *tx;
  Value *v;

  puts("--> char");
  tx = mk_tx("'A'");
  rstring(&v, tx);
  check_char(v, "A");

  tx = mk_tx("'ñ'");
  rstring(&v, tx);
  check_char(v, "ñ");

  tx = mk_tx("'\\''");
  rstring(&v, tx);
  check_char(v, "'");

  tx = mk_tx("'\\$'");
  rstring(&v, tx);
  check_char(v, "\\$");

  tx = mk_tx("'\\u00ff'");
  rstring(&v, tx);
  check_char(v, "\\u00ff");

  puts("--> Str");
  tx = mk_tx("\"A\"");
  rstring(&v, tx);
  check_str(v, "A");

  tx = mk_tx("\"\"");
  rstring(&v, tx);
  check_str(v, "");

  tx = mk_tx("\"ñ\"");
  rstring(&v, tx);
  check_str(v, "ñ");

  tx = mk_tx("\"\\\"\"");
  rstring(&v, tx);
  check_str(v, "\"");

  tx = mk_tx("\"\\$\"");
  rstring(&v, tx);
  check_str(v, "\\$");

  tx = mk_tx("\"\\u00ff\"");
  rstring(&v, tx);
  check_str(v, "\\u00ff");

  tx = mk_tx("\" añ \\\"0\\\" \u45AF \"");
  rstring(&v, tx);
  check_str(v, " añ \"0\" \u45AF ");

  puts("--> Str2");
  tx = mk_tx("`A`");
  rstring(&v, tx);
  check_str2(v, "A");

  tx = mk_tx("``");
  rstring(&v, tx);
  check_str2(v, "");

  tx = mk_tx("`ñ`");
  rstring(&v, tx);
  check_str2(v, "ñ");

  tx = mk_tx("`\\``");
  rstring(&v, tx);
  check_str2(v, "`");

  tx = mk_tx("`\\$`");
  rstring(&v, tx);
  check_str2(v, "\\$");

  tx = mk_tx("`\\u00ff`");
  rstring(&v, tx);
  check_str2(v, "\\u00ff");

  tx = mk_tx("` añ \\`0\\` \u45AF `");
  rstring(&v, tx);
  check_str2(v, " añ `0` \u45AF ");

  tx = mk_tx("`abc ${3 + 4} $inc is \"\\$\"`");
  rstring(&v, tx);
  check_str2(v, "abc ${3 + 4} $inc is \"\\$\"");

  puts("--> Str3");
  tx = mk_tx("\"\"\"\nA\n\"\"\"");
  rstring(&v, tx);
  check_str(v, "A");

  tx = mk_tx("\"\"\"\n\n\"\"\"");
  rstring(&v, tx);
  check_str(v, "");

  tx = mk_tx("\"\"\"\nñ\n\"\"\"");
  rstring(&v, tx);
  check_str(v, "ñ");

  tx = mk_tx("\"\"\"\n\\\"\n\"\"\"");
  rstring(&v, tx);
  check_str(v, "\\\"");

  tx = mk_tx("\"\"\"\n\\$\n\"\"\"");
  rstring(&v, tx);
  check_str(v, "\\$");

  tx = mk_tx("\"\"\"\n\\u00ff\n\"\"\"");
  rstring(&v, tx);
  check_str(v, "\\u00ff");

  tx = mk_tx("\"\"\"\n añ \\\"0\\\" \u45AF \n\"\"\"");
  rstring(&v, tx);
  check_str(v, "añ \\\"0\\\" \u45AF ");

  tx = mk_tx("\"\"\"\n  A\n  B\n  c\n\"\"\"");
  rstring(&v, tx);
  check_str(v, "A\nB\nc");

  puts("    Finished");
}
