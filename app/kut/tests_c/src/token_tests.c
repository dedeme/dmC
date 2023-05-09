// Copyright 07-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "token_tests.h"
#include "kut/DEFS.h"
#include "symix.h"
#include "reader/token.h"

void token_tests () {
  puts(">>> token:");

  Token *b = token_bool(TRUE);
  assert(b->type == TOKEN_BOOL);
  TESTI(b->b, TRUE);
  TEST(token_to_str(b), "Bool: true");
  TEST(token_type_to_str(b), "Bool");

  Token *i = token_int(23);
  assert(i->type == TOKEN_INT);
  TESTI(i->i, 23);
  TEST(token_to_str(i), "Int: 23");
  TEST(token_type_to_str(i), "Int");

  Token *f = token_float(-12.450);
  assert(f->type == TOKEN_FLOAT);
  TESTF(f->d, -12.45);
  TEST(token_to_str(f), "Float: -12.45");
  TEST(token_type_to_str(f), "Float");

  f = token_float(-12.e-1);
  assert(f->type == TOKEN_FLOAT);
  TESTF(f->d, -1.2);
  TEST(token_to_str(f), "Float: -1.2");
  TEST(token_type_to_str(f), "Float");

  Token *s = token_string("ab\\c");
  assert(s->type == TOKEN_STRING);
  TEST(s->value, "ab\\c");
  TEST(token_to_str(s), "String: \"ab\\\\c\"");
  TEST(token_type_to_str(s), "String");

  Token *lc = token_line_comment("/*so\nandso*/");
  assert(lc->type == TOKEN_LINE_COMMENT);
  TEST(lc->value, "/*so\nandso*/");
  TEST(token_to_str(lc), "Line_comment: /*so\nandso*/");
  TEST(token_type_to_str(lc), "Line_comment");

  Token *c = token_comment("// so");
  assert(c->type == TOKEN_COMMENT);
  TEST(c->value, "// so");
  TEST(token_to_str(c), "Comment: // so");
  TEST(token_type_to_str(c), "Comment");

  Token *sy = token_symbol("if");
  assert(sy->type == TOKEN_SYMBOL);
  TEST(symix_get(sy->b), "if");
  TEST(token_to_str(sy), "Symbol: if");
  TEST(token_type_to_str(sy), "Symbol");

  Token *o = token_operator("+");
  assert(o->type == TOKEN_OPERATOR);
  TEST(o->value, "+");
  TEST(token_to_str(o), "Operator: +");
  TEST(token_type_to_str(o), "Operator");

  assert(token_is_unary(token_operator("-")));
  assert(!token_is_unary(token_operator(":")));
  assert(!token_is_unary(token_int(1)));

  assert(token_is_binary(token_operator("-")));
  assert(!token_is_binary(token_operator(":")));
  assert(!token_is_binary(token_int(1)));

  assert(token_is_binary1(token_operator("%")));
  assert(!token_is_binary1(token_operator(":")));
  assert(!token_is_binary1(token_int(1)));

  assert(token_is_binary2(token_operator("-")));
  assert(!token_is_binary2(token_operator(":")));
  assert(!token_is_binary2(token_int(1)));

  assert(token_is_binary3(token_operator("==")));
  assert(!token_is_binary3(token_operator(":")));
  assert(!token_is_binary3(token_int(1)));

  assert(token_is_binary4(token_operator("|")));
  assert(!token_is_binary4(token_operator(":")));
  assert(!token_is_binary4(token_int(1)));

  assert(token_is_ternary(token_operator("?")));
  assert(!token_is_ternary(token_operator(":")));
  assert(!token_is_ternary(token_int(1)));

  assert(token_is_assign(token_operator("-=")));
  assert(!token_is_assign(token_operator(":")));
  assert(!token_is_assign(token_int(1)));

  assert(token_is_equals(token_operator("=")));
  assert(!token_is_equals(token_operator(":")));
  assert(!token_is_equals(token_int(1)));

  assert(token_is_comma(token_operator(",")));
  assert(!token_is_comma(token_operator(":")));
  assert(!token_is_comma(token_int(1)));

  assert(token_is_colon(token_operator(":")));
  assert(!token_is_colon(token_operator(";")));
  assert(!token_is_colon(token_int(1)));

  assert(token_is_semicolon(token_operator(";")));
  assert(!token_is_semicolon(token_operator(":")));
  assert(!token_is_semicolon(token_int(1)));

  assert(token_is_backslash(token_operator("\\")));
  assert(!token_is_backslash(token_operator(":")));
  assert(!token_is_backslash(token_int(1)));

  assert(token_is_arrow(token_operator("->")));
  assert(!token_is_arrow(token_operator(":")));
  assert(!token_is_arrow(token_int(1)));

  assert(token_is_open_par(token_operator("(")));
  assert(!token_is_open_par(token_operator(":")));
  assert(!token_is_open_par(token_int(1)));

  assert(token_is_close_par(token_operator(")")));
  assert(!token_is_close_par(token_operator(":")));
  assert(!token_is_close_par(token_int(1)));

  assert(token_is_open_square(token_operator("[")));
  assert(!token_is_open_square(token_operator(":")));
  assert(!token_is_open_square(token_int(1)));

  assert(token_is_close_square(token_operator("]")));
  assert(!token_is_close_square(token_operator(":")));
  assert(!token_is_close_square(token_int(1)));

  assert(token_is_open_bracket(token_operator("{")));
  assert(!token_is_open_bracket(token_operator(":")));
  assert(!token_is_open_bracket(token_int(1)));

  assert(token_is_close_bracket(token_operator("}")));
  assert(!token_is_close_bracket(token_operator(":")));
  assert(!token_is_close_bracket(token_int(1)));

  assert(token_is_else(token_symbol("else")));
  assert(!token_is_else(token_symbol("if")));
  assert(!token_is_else(token_int(1)));

  puts("  ... Finished");
}
