// Copyright 11-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "tests/all.h"
#include "readers/tokenRd.h"
#include "kut/js.h"

static char *to_js (Token *this) {
  return js_wa(arr_new_from(
    js_wi(this->ln),
    js_wi(this->tp),
    js_ws(this->value),
    NULL
  ));
}

static void test (Token *actual, Token *expected) {
  if (strcmp(to_js(actual),to_js(expected)))
    EXC_KUTT(str_f(
      "Test failed:\nExpected: %s\n  Actual: %s",
      to_js(expected), to_js(actual)
    ));
}

static Token *mk (int ln, int tp, char *value) {
  return token_new(ln, tp, value);
}

static TokenRd *rd = NULL;

static Token *next() {
  return tokenRd_next(rd);
}

void ttoken_run() {
  puts("token:");

  rd = tokenRd_new("  false  ");
  test(next(), mk(1, token_bool, "false"));
  test(next(), mk(1, token_eof, ""));
  test(next(), mk(1, token_eof, ""));

  rd = tokenRd_new("  true  /* \n x */ True // ab \n // bc \n1 \n");
  test(next(), mk(1, token_bool, "true"));
  //test(next(), mk(1, token_comment, " \n x "));
  test(next(), mk(2, token_symbol, "True"));
  //test(next(), mk(2, token_lineComment, " ab "));
  //test(next(), mk(3, token_lineComment, " bc "));
  test(next(), mk(4, token_int, "1"));
  test(next(), mk(5, token_eof, ""));

  rd = tokenRd_new(
    "1 1.2 +1 -12.2 23.4e2 \n\n 3.45E-4 0.00e+4 \n 1_124  1_124.2e-2"
  );
  test(next(), mk(1, token_int, "1"));
  test(next(), mk(1, token_float, "1.2"));
  test(next(), mk(1, token_operator, "+"));
  test(next(), mk(1, token_int, "1"));
  test(next(), mk(1, token_operator, "-"));
  test(next(), mk(1, token_float, "12.2"));
  test(next(), mk(1, token_float, "23.4e2"));
  test(next(), mk(3, token_float, "3.45E-4"));
  test(next(), mk(3, token_float, "0.00e+4"));
  test(next(), mk(4, token_int, "1124"));
  test(next(), mk(4, token_float, "1124.2e-2"));
  test(next(), mk(4, token_eof, ""));

  rd = tokenRd_new("a11_2e2\n");
  test(next(), mk(1, token_symbol, "a11"));
  test(next(), mk(1, token_operator, "_"));
  test(next(), mk(1, token_int, "2"));
  test(next(), mk(1, token_symbol, "e2"));
  test(next(), mk(2, token_eof, ""));

  rd = tokenRd_new("'' 'añb\\n\\t\\f\\r\\\\d\\u0064c\\'x\\''");
  test(next(), mk(1, token_string, ""));
  test(next(), mk(1, token_string, "añb\n\t\f\r\\ddc\'x\'"));
  test(next(), mk(1, token_eof, ""));

  rd = tokenRd_new("\"\" \"añb\\n\\t\\f\\r\\\\d\\u0064c\\\"x\\\"\"");
  test(next(), mk(1, token_string, ""));
  test(next(), mk(1, token_string, "añb\n\t\f\r\\ddc\"x\""));
  test(next(), mk(1, token_eof, ""));

  rd = tokenRd_new("\"\"\"\n  añb \n   c\n  d\n\"\"\" \"\"\"\"\"\" \"\"\"abñc\"\"\"");
  test(next(), mk(1, token_string, "añb \n c\nd\n"));
  test(next(), mk(5, token_string, ""));
  test(next(), mk(5, token_string, "abñc"));
  test(next(), mk(5, token_eof, ""));

  rd = tokenRd_new(
    "= == === <= >= >> << - -> -= \\ + += / /= * *= % %= & &= | |= != !!"
  );
  Arr *list = arr_new_from(
    "=", "==", "==", "=", "<=", ">=", ">", ">", "<", "<", "-", "->", "-=",
    "\\", "+", "+=", "/", "/=", "*", "*=", "%", "%=", "&", "&=",
    "|", "|=", "!=", "!", "!", NULL
    );
  EACH(list, char, o) {
    test(next(), mk(1, token_operator, o));
  }_EACH
  test(next(), mk(1, token_eof, ""));

  rd = tokenRd_new("\n'a\n'");
  test(next(), mk(2, 0, "Unclosed quotes."));

  rd = tokenRd_new("\"a\nb\"");
  test(next(), mk(1, 0, "Unclosed quotes."));
  rd = tokenRd_new("'a\\cv'");
  test(next(), mk(1, 0, "Bad escape sequence 'c'."));

  puts("  finished");
}
