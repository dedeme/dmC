// Copyright 08-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "token_tests.h"
#include "kut/DEFS.h"
#include "symix.h"
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
  TEST(symix_get(tk_rs->b), "v");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "=");
  tk_rs = cdr_read_token(rd);
  assert(tk_rs->b);
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, ";");

  TRY {
    cdr_read_token(rd);
    assert(FALSE);
  } CATCH (e) {
    assert(e);
  }_TRY

  rd = cdr_new(0, " 0 != false ;   ");
  tk_rs = cdr_read_token(rd);
  TESTI(tk_rs->i, 0);
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "!=");
  tk_rs = cdr_read_token(rd);
  assert(!tk_rs->b);
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, ";");

  rd = cdr_new(0, "  1 == -13  ");
  tk_rs = cdr_read_token(rd);
  TESTI(tk_rs->i, 1);
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "==");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "-");
  tk_rs = cdr_read_token(rd);
  TESTI(tk_rs->i, 13);

  rd = cdr_new(0, "022 < - 013  ");
  tk_rs = cdr_read_token(rd);
  TESTI(tk_rs->i, 22);
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "<");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "-");
  tk_rs = cdr_read_token(rd);
  TESTI(tk_rs->i, 13);

  rd = cdr_new(0, "1_022 < - 1_013  ");
  tk_rs = cdr_read_token(rd);
  TESTI(tk_rs->i, 1022);
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "<");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "-");
  tk_rs = cdr_read_token(rd);
  TESTI(tk_rs->i, 1013);

  rd = cdr_new(0, "- 2.0 <= 13.  ");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "-");
  tk_rs = cdr_read_token(rd);
  TESTF(tk_rs->d, 2.0);
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "<=");
  tk_rs = cdr_read_token(rd);
  TESTF(tk_rs->d, 13.0);

  rd = cdr_new(0, "1_342.02 > 13.150;  ");
  tk_rs = cdr_read_token(rd);
  TESTF(tk_rs->d, 1342.02);
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, ">");
  tk_rs = cdr_read_token(rd);
  TESTF(tk_rs->d, 13.15);

  rd = cdr_new(0, "1_342.02e2 >= 13.150e-2");
  tk_rs = cdr_read_token(rd);
  TESTF(tk_rs->d, 134202);
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, ">=");
  tk_rs = cdr_read_token(rd);
  TESTF(tk_rs->d, 0.1315);

  rd = cdr_new(0, "1_342.02e+2 + 13.150e-2 - 0;");
  tk_rs = cdr_read_token(rd);
  TESTF(tk_rs->d, 134202);
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "+");
  tk_rs = cdr_read_token(rd);
  TESTF(tk_rs->d, 0.1315);
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "-");
  tk_rs = cdr_read_token(rd);
  TESTI(tk_rs->i, 0);

  TRY { cdr_new(0, "\n\n1_342.02e--2 + 13.150e-2 - 0;"); assert(FALSE); }
  CATCH (e) { assert (e); }
  _TRY

  rd = cdr_new(0, "fx * dzz / 4 % 2 ;");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "fx");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "*");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "dzz");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "/");
  tk_rs = cdr_read_token(rd);
  TESTI(tk_rs->i, 4);
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "%");
  tk_rs = cdr_read_token(rd);
  TESTI(tk_rs->i, 2);
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, ";");

  rd = cdr_new(0, "true & fals | tru ;");
  tk_rs = cdr_read_token(rd);
  assert(tk_rs->b);
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "&");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "fals");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "|");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "tru");

  rd = cdr_new(0, "\\ x -> f = !f.bar[z3](a, b) ;");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "\\");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "x");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "->");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "f");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "=");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "!");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "f");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, ".");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "bar");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "[");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "z3");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "]");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "(");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "a");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, ",");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "b");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, ")");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, ";");

  rd = cdr_new(0, "{a ? a : b}");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "{");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "a");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "?");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "a");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, ":");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "b");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "}");

  rd = cdr_new(0, "a = \"\"");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "a");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "=");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "");

  rd = cdr_new(0, "a += \"<\"");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "a");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "+=");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "<");

  rd = cdr_new(0, "a -= \"a\\\"b\\\\c\\nnew cañón\"");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "a");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "-=");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "a\"b\\c\nnew cañón");

  rd = cdr_new(0, "a *= \"euro = \\u20aC\"");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "a");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "*=");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "euro = €");

  TRY { cdr_new(0, "\"euro = \\ u20aC\""); assert (FALSE); }
  CATCH(e) { assert(e); }
  _TRY

  rd = cdr_new(0, "\"\"\"  euro = \\ u20aC\n   and more.\"\"\"");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "euro = \\ u20aC\n and more.");

  rd = cdr_new(0, "\"\"\"\n  euro = \\ u20aC\n   and more.\"\"\"");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "euro = \\ u20aC\n and more.");

  rd = cdr_new(0, "\"\"\"\n\n  euro = \\ u20aC\n   and more.\"\"\"");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "\neuro = \\ u20aC\n and more.");

  rd = cdr_new(0, "a\n//abc\nb");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "a");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "b");

  rd = cdr_new(0, "a//abc\nb");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "a");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "b");

  rd = cdr_new(0, "a/*a\nbc*/b");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "a");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "b");

  rd = cdr_new(0, "a/**/b");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "a");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "b");

  rd = cdr_new(0, "a/* * /*/b");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "a");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "b");

  rd = cdr_new(0, "r /= 4");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "r");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "/=");
  tk_rs = cdr_read_token(rd);
  TESTI(tk_rs->i, 4);

  rd = cdr_new(0, "r %= 4 / 3");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "r");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "%=");
  tk_rs = cdr_read_token(rd);
  TESTI(tk_rs->i, 4);
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "/");
  tk_rs = cdr_read_token(rd);
  TESTI(tk_rs->i, 3);

  rd = cdr_new(0, "r |= true");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "r");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "|=");
  tk_rs = cdr_read_token(rd);
  assert(tk_rs->b);

  rd = cdr_new(0, "r &= false");
  tk_rs = cdr_read_token(rd);
  TEST(symix_get(tk_rs->b), "r");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "&=");
  tk_rs = cdr_read_token(rd);
  assert(!tk_rs->b);

  rd = cdr_new(0, "::=;");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, ":");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, ":");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, "=");
  tk_rs = cdr_read_token(rd);
  TEST(tk_rs->value, ";");

  puts("  ... Finished");
}
