// Copyright 23-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "data/wrSRs.h"
#include "data/wrERs.h"

static WrSRs *new (int is_error, WrSRsStopT stopped, char *code) {
  WrSRs *this = MALLOC(WrSRs);
  this->is_error = is_error;
  this->stopped = stopped;
  this->code = code;
  return this;
}

WrSRs *wrSRs_fail (WrCtx *ctx, int ln, char *msg) {
  char *fmap (char *l) { return str_f("  %s", l); }
  return new(TRUE, wrSRs_no, str_f("%s:%d:\n%s",
    ctx->md_path, ln,
    arr_cjoin(arr_map(str_csplit(msg, '\n'), (FMAP)fmap), '\n')
  ));
}

WrSRs *wrSRs_failE (WrCtx *ctx, int ln, char *expected, char *actual) {
  return wrSRs_fail(ctx, ln, str_f("Expected: %s\n  Actual: %s",
    expected, actual
  ));
}

WrSRs *wrSRs_failT (WrCtx *ctx, int ln, Type *t, Exp *e, Type *et) {
  return wrSRs_failE(ctx, ln,
    str_f("Expression of type %s", type_to_str(t)),
    str_f("%s (of type %s)", exp_to_str(e), type_to_str(et))
  );
}

WrSRs *wrSRs_fromWrERsFail (WrERs *rs) {
  return new(TRUE, wrSRs_no, rs->code);
}

WrSRs *wrSRs_mk (WrSRsStopT stopped, char *code) {
  return new(FALSE, stopped, code);
}
