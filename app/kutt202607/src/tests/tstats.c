// Copyright 14-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "tests/tstats.h"
#include "data/stat.h"
#include "readers/stRd.h"
#include "data/rdCtx.h"
#include "kut/js.h"

#define TST(actualSt, expectedStr) {\
  Stat *st = actualSt; \
  if (strcmp(stat_to_str(st),expectedStr)) \
    EXC_KUTT(emsg(stat_to_str(st), expectedStr)); \
  }

#define TST_TK(actualToken, expectedStr) {\
  Token *tk = actualToken; \
  if (strcmp(token_to_str(tk),expectedStr)) \
    EXC_KUTT(emsg(token_to_str(tk), expectedStr)); \
  }

static char *emsg (char *actual, char *expected) {
  return str_f(
    "Test failed:\nExpected: %s\n  Actual: %s",
    js_ws(expected), js_ws(actual)
  );
}

static StRd *mkRd (char *tx) {
  RdCtx *ctx = rdCtx_new("a/b", "md12");
  return stRd_new(ctx, tx);
};

static Stat *next (StRd *rd) {
  return stRd_next(rd);
}

void tstats_run() {
  puts("tstat:");

  StRd *rd = mkRd(" { } \n a = 13 \n; :md c = 'abc' ; j = c.test();");
  TST(next(rd), "{\n\n}");
  TST(next(rd), "a=13");
  TST(next(rd), "c=\"abc\"");
  TST(next(rd), "j=md.test(c)");
  TST(next(rd), "Unexpected end of file");
  TST(next(rd), "Unexpected end of file");

  rd = mkRd("A = []/i/; \n  D = {}/<file>/;");
  TST(next(rd), "A=[]/i/");
  TST(next(rd), "D={}/<file>/");

  rd = mkRd(" { a = 13; :md c = true \n;} , a = fc(); ");
  TST(next(rd), "{\na=13\nc=true\n}");
  TST(next(rd), ",a=fc()");

  rd = mkRd(" //:[. iii] \n a : bb, dd, cc; a :; z = b() ");
  TST(next(rd), "a:[.iii]bb,dd,cc");
  TST(next(rd), "Expected: symbol\n  Actual: Operator: ;");

  rd = mkRd(" a[1]=z; a [ 1 ] += z ; a[1]-=z;a[1]*=z;a[1]/=z;a[1]%=z;");
  TST(next(rd), "a[1]=z");
  TST(next(rd), "a[1]+=z");
  TST(next(rd), "a[1]-=z");
  TST(next(rd), "a[1]*=z");
  TST(next(rd), "a[1]/=z");
  TST(next(rd), "a[1]%=z");

  rd = mkRd(" a[1]&=z; a [ 1 ] |= z ; f(ab)()[2] -= 11;;");
  TST(next(rd), "a[1]&=z");
  TST(next(rd), "a[1]|=z");
  TST(next(rd), "f(ab)()[2]-=11");
  TST(next(rd), "Unexpected 'Operator: ;'");

  rd = mkRd(" fn( ); fn ( a , b); md.fn(); md.fn(a,b,c);");
  TST(next(rd), "fn()");
  TST(next(rd), "fn(a,b)");
  TST(next(rd), "md.fn()");
  TST(next(rd), "md.fn(a,b,c)");

  rd = mkRd(" v[3]( ); v[3] ( a , b); md.v[2](); md.v[4](a,b,c);");
  TST(next(rd), "v[3]()");
  TST(next(rd), "v[3](a,b)");
  TST(next(rd), "md.v[2]()");
  TST(next(rd), "md.v[4](a,b,c)");

  rd = mkRd(" break ; continue; return  ;  return fn(a.b);");
  TST(next(rd), "break");
  TST(next(rd), "continue");
  TST(next(rd), "return");
  TST(next(rd), "return fn(a.b)");

  rd = mkRd("  while () {} while (a) return; while (true) {return;} ;");
  TST(next(rd), "while(){\n\n}");
  TST(next(rd), "while(a){\nreturn\n}");
  TST(next(rd), "while(true){\nreturn\n}");
  TST(next(rd), "Unexpected 'Operator: ;'");

  rd = mkRd("  while () {a = b + 5; c = 3; return a+c;}");
  TST(next(rd), "while(){\na=(b+5)\nc=3\nreturn (a+c)\n}");

  rd = mkRd("  if (x) a(); if (x){a=2;return a; } if (true){}");
  TST(next(rd), "if(x){\na()\n}");
  TST(next(rd), "if(x){\na=2\nreturn a\n}");
  TST(next(rd), "if(true){\n\n}");

  rd = mkRd("  if (x) a(); else b(); if(x){}else{} if (x){a();} else {b();}");
  TST(next(rd), "if(x){\na()\n}else {\nb()\n}");
  TST(next(rd), "if(x){\n\n}else {\n\n}");
  TST(next(rd), "if(x){\na()\n}else {\nb()\n}");

  puts("  finished");
}

#undef TST
#undef TST_TK
