// Copyright 14-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "tests/texps.h"
#include "readers/expRd.h"
#include "data/rdCtx.h"

#define TST(actualExp, expectedStr) {\
  Exp *tp = actualExp; \
  if (strcmp(exp_to_str(tp),expectedStr)) \
    EXC_KUTT(emsg(exp_to_str(tp), expectedStr)); \
  }

#define TST_TK(actualToken, expectedStr) {\
  Token *tk = actualToken; \
  if (strcmp(token_to_str(tk),expectedStr)) \
    EXC_KUTT(emsg(token_to_str(tk), expectedStr)); \
  }

static char *emsg (char *actual, char *expected) {
  return str_f(
    "Test failed:\nExpected: %s\n  Actual: %s",
    expected, actual
  );
}

static ExpRd *mkRd (char *tx) {
  RdCtx *ctx = rdCtx_new("a/b", "md12");
  return expRd_new(ctx, tx);
};

static Exp *next (ExpRd *rd) {
  return expRd_next(rd);
}

void texps_run() {
  puts("texp:");

  ExpRd *rd = mkRd(" true ");
  TST(next(rd), "true");
  TST(next(rd), "Unexpected end of file");

  rd = mkRd(" tr4f}ue ");
  TST(next(rd), "tr4f");
  TST(next(rd), "Unexpected '}'");

  rd = mkRd("12 3.15 'abcà' \"defñ\"");
  TST(next(rd), "12");
  TST(next(rd), "3.15");
  TST(next(rd), "\"abcà\"");
  TST(next(rd), "\"defñ\"");
  TST(next(rd), "Unexpected end of file");

  rd = mkRd(" [ 1, 2] ; []/i/ ;");
  TST(next(rd), "[1,2]");
  TST_TK(expRd_next_token(rd), "Operator: ;");
  TST(next(rd), "[]/i/");

  rd = mkRd(" {a:1, b:2} ");
  TST(next(rd), "{\"a\":1,\"b\":2}");

  rd = mkRd(" [. 1, 2] ; [. abc12] ; [.] ");
  TST(next(rd), "[.1,2]");
  TST_TK(expRd_next_token(rd), "Operator: ;");
  TST(next(rd), "[.abc12]");
  TST_TK(expRd_next_token(rd), "Operator: ;");
  TST(next(rd), "Unexpected ']'");

  rd = mkRd(" [! 1, 2] ; [! 3,3,-5] ; [!2,] ");
  TST(next(rd), "[!1,2]");
  TST_TK(expRd_next_token(rd), "Operator: ;");
  TST(next(rd), "[!3,3,-5]");
  TST_TK(expRd_next_token(rd), "Operator: ;");
  TST(next(rd), "Unexpected ']'");

  rd = mkRd("switch (v) {2, 4: true; 1: false; default: false; }");
  TST(next(rd), "switch(v){2,4:true;1:false;default:false}");
  TST(next(rd), "Unexpected end of file");

  rd = mkRd(" a . b a[1] a.b[ 3: -6] a! c[]");
  TST(next(rd), "a.b");
  TST(next(rd), "a[1]");
  TST(next(rd), "a.b[3:-6]");
  TST(next(rd), "a[0]");
  TST(next(rd), "Unexpected ']'");

  rd = mkRd(" fn ( ) fn2(1 ) fn2(3, a.b)");
  TST(next(rd), "fn()");
  TST(next(rd), "fn2(1)");
  TST(next(rd), "fn2(3,a.b)");

  rd = mkRd(" fn ( )( ) fn()(a)[4] fn[a](a)[n]() fn[a](a)[n]()[z] ");
  TST(next(rd), "fn()()");
  TST(next(rd), "fn()(a)[4]");
  TST(next(rd), "fn[a](a)[n]()");
  TST(next(rd), "fn[a](a)[n]()[z]");

  rd = mkRd(" a ? 3 : -1 b ? {a}");
  TST(next(rd), "a?3:-1");
  TST(next(rd), "Expected: :\n  Actual: End_of_file");

  rd = mkRd(" a * b / c   a + b - c    a - b * c");
  TST(next(rd), "((a*b)/c)");
  TST(next(rd), "((a+b)-c)");
  TST(next(rd), "(a-(b*c))");

  rd = mkRd(" a! & b >= c[3+c] | (3 > --s.b + 2*6)");
  TST(next(rd), "((a[0]&(b>=c[(3+c)]))|((3>(--s.b+(2*6)))))");
  TST(next(rd), "Unexpected end of file");

  puts("  finished");
}

#undef TST
#undef TST_TK
