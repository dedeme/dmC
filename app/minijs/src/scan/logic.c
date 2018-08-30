// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "scan/logic.h"
#include "dmc/Tuples.h"
#include "dmc/Buf.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "scan/token.h"

ScanResult *logic_and(AstArr *a, Scanner *sc) {
  Arr *rs = arr_new();
  Fails *fs = fails_new();
  EACH(a, AstTp, tp) {
    void *context = ast_tp_context(tp);
    AstFn f = ast_tp_fn(tp);
    ScanResult *r = f(context, sc);
    if (scanResult_is_error(r)) {
      return scanResult_error(scanResult_fails(r));
    }
    fails_add_warnings(fs, scanResult_warnings(r));
    arr_add(rs, scanResult_result(r));
    sc = scanResult_scanner(r);
  }_EACH
  return scanResult_new(fs, rs, sc);
}

ScanResult *logic_or(AstArr *a, Scanner *sc) {
  Fails *fs = fails_new();
  EACH(a, AstTp, tp) {
    void *context = ast_tp_context(tp);
    AstFn f = ast_tp_fn(tp);
    ScanResult *r = f(context, sc);
    if (!scanResult_is_error(r)) {
      return r;
    }
    fails_add_errors(fs, fails_errors(scanResult_fails(r)));
  }_EACH
  char *err = "Several options failed:\n";
  EACH(fails_errors(fs), Fail, e) {
    err = str_cat(err, "  ", fail_msg(e), NULL);
  }_EACH
  return scanResult_error(fails_new_error(scanner_mk_fail(sc, err)));
}

ScanResult *logic_none_or_several (AstTp *tp, Scanner *sc) {
  void *context = ast_tp_context(tp);
  AstFn f = ast_tp_fn(tp);
  Arr *rs = arr_new();
  Fails *fs = fails_new();
  while (true) {
    ScanResult *r = f(context, sc);
    if (scanResult_is_error(r)) {
      return scanResult_new(fs, rs, sc);
    }
    fails_add_warnings(fs, scanResult_warnings(r));
    arr_add(rs, scanResult_result(r));
    sc = scanResult_scanner(r);
  }
}

ScanResult *logic_one_or_more (AstTp *tp, Scanner *sc) {
  void *context = ast_tp_context(tp);
  AstFn f = ast_tp_fn(tp);
  Arr *rs = arr_new();
  Fails *fs = fails_new();
  while (true) {
    ScanResult *r = f(context, sc);
    if (scanResult_is_error(r)) {
      if (arr_size(rs) == 0) {
        return r;
      }
      return scanResult_new(fs, rs, sc);
    }
    fails_add_warnings(fs, scanResult_warnings(r));
    arr_add(rs, scanResult_result(r));
    sc = scanResult_scanner(r);
  }
}

ScanResult *logic_one_or_none (AstTp *tp, Scanner *sc) {
  void *context = ast_tp_context(tp);
  AstFn f = ast_tp_fn(tp);

  ScanResult *r = f(context, sc);
  if (scanResult_is_error(r)) {
    return scanResult_new_ok(opt_null(), sc);
  }
  return scanResult_new(
    scanResult_fails(r),
    opt_new(scanResult_result(r)),
    scanResult_scanner(r)
  );
}

ScanResult *logic_not(AstMsg *tp, Scanner *sc) {
  AstTp *tp2 = ast_msg_cast(tp);
  void *context = ast_tp_context(tp2);
  AstFn f = ast_tp_fn(tp2);

  ScanResult *r = f(context, sc);
  if (scanResult_is_error(r)) {
    return scanResult_new_ok(opt_null(), scanner_next(sc));
  }
  char *msg = ast_msg_msg(tp);
  return scanResult_error(fails_new_error(scanner_mk_fail(sc, msg)));
}

ScanResult *logic_not0(char *s, Scanner *scanner) {
  char open = s[0];
  char close = s[1];
  int c = 0;

  Scanner *init = scanner;
  Buf *bf = buf_new();
  while (true) {
    char ch = scanner_char(scanner);
    if (ch) {
      if (ch == close && c == 0) {
        return scanResult_new_ok(buf_to_str(bf), scanner_next(scanner));
      }
      if (ch == close) {
        --c;
      }
      if (ch == open) {
        ++c;
      }
      buf_cadd(bf, ch);
      scanner = scanner_next(scanner);
    } else {
      return scanResult_error(fails_new_error(scanner_mk_fail(
        init, str_printf("'%c' without closing", open)
      )));
    }
  }

}

