// Copyright 08-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "token_tests.h"
#include "kut/DEFS.h"
#include "reader/cdr/cdr.h"

void cdr_tests () {
  puts(">>> cdr:");

  Cdr *rd = cdr_new(0, "");
  TRY {
    cdr_read_token(rd);
    assert(FALSE);
  } CATCH (e) {
    assert(e);
  }_TRY

  rd = cdr_new(0, "v = true;");
  Token *tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "v");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "=");
  tk_rs = cdr_read_token(rd);
  assert(token_get_bool(tk_rs));
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), ";");

  TRY {
    cdr_read_token(rd);
    assert(FALSE);
  } CATCH (e) {
    assert(e);
  }_TRY

  rd = cdr_new(0, " 0 != false ;   ");
  tk_rs = cdr_read_token(rd);
  TESTI(token_get_int(tk_rs), 0);
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "!=");
  tk_rs = cdr_read_token(rd);
  assert(!token_get_bool(tk_rs));
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), ";");

  rd = cdr_new(0, "  1 == -13  ");
  tk_rs = cdr_read_token(rd);
  TESTI(token_get_int(tk_rs), 1);
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "==");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "-");
  tk_rs = cdr_read_token(rd);
  TESTI(token_get_int(tk_rs), 13);

  rd = cdr_new(0, "022 < - 013  ");
  tk_rs = cdr_read_token(rd);
  TESTI(token_get_int(tk_rs), 22);
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "<");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "-");
  tk_rs = cdr_read_token(rd);
  TESTI(token_get_int(tk_rs), 13);

  rd = cdr_new(0, "1_022 < - 1_013  ");
  tk_rs = cdr_read_token(rd);
  TESTI(token_get_int(tk_rs), 1022);
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "<");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "-");
  tk_rs = cdr_read_token(rd);
  TESTI(token_get_int(tk_rs), 1013);

  rd = cdr_new(0, "- 2.0 <= 13.  ");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "-");
  tk_rs = cdr_read_token(rd);
  TESTF(token_get_float(tk_rs), 2.0);
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "<=");
  tk_rs = cdr_read_token(rd);
  TESTF(token_get_float(tk_rs), 13.0);

  rd = cdr_new(0, "1_342.02 > 13.150;  ");
  tk_rs = cdr_read_token(rd);
  TESTF(token_get_float(tk_rs), 1342.02);
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), ">");
  tk_rs = cdr_read_token(rd);
  TESTF(token_get_float(tk_rs), 13.15);

  rd = cdr_new(0, "1_342.02e2 >= 13.150e-2");
  tk_rs = cdr_read_token(rd);
  TESTF(token_get_float(tk_rs), 134202);
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), ">=");
  tk_rs = cdr_read_token(rd);
  TESTF(token_get_float(tk_rs), 0.1315);

  rd = cdr_new(0, "1_342.02e+2 + 13.150e-2 - 0;");
  tk_rs = cdr_read_token(rd);
  TESTF(token_get_float(tk_rs), 134202);
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "+");
  tk_rs = cdr_read_token(rd);
  TESTF(token_get_float(tk_rs), 0.1315);
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "-");
  tk_rs = cdr_read_token(rd);
  TESTI(token_get_int(tk_rs), 0);

  TRY { cdr_new(0, "\n\n1_342.02e--2 + 13.150e-2 - 0;"); assert(FALSE); }
  CATCH (e) { assert (e); }
  _TRY

  rd = cdr_new(0, "fx * dzz / 4 % 2 ;");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "fx");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "*");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "dzz");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "/");
  tk_rs = cdr_read_token(rd);
  TESTI(token_get_int(tk_rs), 4);
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "%");
  tk_rs = cdr_read_token(rd);
  TESTI(token_get_int(tk_rs), 2);
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), ";");

  rd = cdr_new(0, "true & fals | tru ;");
  tk_rs = cdr_read_token(rd);
  assert(token_get_bool(tk_rs));
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "&");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "fals");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "|");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "tru");

  rd = cdr_new(0, "\\ x -> f = !f.bar[z3](a, b) ;");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "\\");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "x");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "->");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "f");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "=");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "!");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "f");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), ".");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "bar");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "[");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "z3");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "]");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "(");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "a");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), ",");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "b");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), ")");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), ";");

  rd = cdr_new(0, "{a ? a : b}");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "{");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "a");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "?");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "a");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), ":");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "b");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "}");

  rd = cdr_new(0, "a = \"\"");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "a");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "=");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_string(tk_rs), "");

  rd = cdr_new(0, "a += \"<\"");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "a");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "+=");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_string(tk_rs), "<");

  rd = cdr_new(0, "a -= \"a\\\"b\\\\c\\nnew cañón\"");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "a");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "-=");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_string(tk_rs), "a\"b\\c\nnew cañón");

  rd = cdr_new(0, "a *= \"euro = \\u20aC\"");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "a");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "*=");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_string(tk_rs), "euro = €");

  TRY { cdr_new(0, "\"euro = \\ u20aC\""); assert (FALSE); }
  CATCH(e) { assert(e); }
  _TRY

  rd = cdr_new(0, "\"\"\"  euro = \\ u20aC\n   and more.\"\"\"");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_string(tk_rs), "euro = \\ u20aC\n and more.");

  rd = cdr_new(0, "\"\"\"\n  euro = \\ u20aC\n   and more.\"\"\"");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_string(tk_rs), "euro = \\ u20aC\n and more.");

  rd = cdr_new(0, "\"\"\"\n\n  euro = \\ u20aC\n   and more.\"\"\"");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_string(tk_rs), "\neuro = \\ u20aC\n and more.");

  rd = cdr_new(0, "a\n//abc\nb");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "a");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "b");

  rd = cdr_new(0, "a//abc\nb");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "a");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "b");

  rd = cdr_new(0, "a/*a\nbc*/b");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "a");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "b");

  rd = cdr_new(0, "a/**/b");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "a");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "b");

  rd = cdr_new(0, "a/* * /*/b");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "a");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "b");

  rd = cdr_new(0, "r /= 4");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "r");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "/=");
  tk_rs = cdr_read_token(rd);
  TESTI(token_get_int(tk_rs), 4);

  rd = cdr_new(0, "r %= 4 / 3");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "r");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "%=");
  tk_rs = cdr_read_token(rd);
  TESTI(token_get_int(tk_rs), 4);
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "/");
  tk_rs = cdr_read_token(rd);
  TESTI(token_get_int(tk_rs), 3);

  rd = cdr_new(0, "r |= true");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "r");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "|=");
  tk_rs = cdr_read_token(rd);
  assert(token_get_bool(tk_rs));

  rd = cdr_new(0, "r &= false");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_symbol(tk_rs), "r");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "&=");
  tk_rs = cdr_read_token(rd);
  assert(!token_get_bool(tk_rs));

  rd = cdr_new(0, "::=;");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), ":");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), ":");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), "=");
  tk_rs = cdr_read_token(rd);
  TEST(token_get_operator(tk_rs), ";");

  puts("  ... Finished");
}
