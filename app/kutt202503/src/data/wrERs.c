// Copyright 23-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "data/wrERs.h"

static WrERs *new (int is_error, Type *tp, char *fun_code, char *code) {
  WrERs *this = MALLOC(WrERs);
  this->is_error = is_error;
  this->tp =tp;
  this->fun_code = fun_code;
  this->code = code;
  return this;
}

WrERs *wrERs_fail (WrCtx *ctx, int ln, char *msg) {
  char *fmap (char *l) { return str_f("  %s", l); }
  return new(TRUE, type_bool(), "", str_f("%s:%d:\n%s",
    ctx->md_path, ln,
    arr_cjoin(arr_map(str_csplit(msg, '\n'), (FMAP)fmap), '\n')
  ));
}

WrERs *wrERs_failE (WrCtx *ctx, int ln, char *expected, char *actual) {
  return wrERs_fail(ctx, ln, str_f("Expected: %s\n  Actual: %s",
    expected, actual
  ));
}

WrERs *wrERs_failT (WrCtx *ctx, int ln, Type *t, Exp *e, Type *et) {
  return wrERs_failE(ctx, ln,
    str_f("Expression of type %s", type_to_str(t)),
    str_f("%s (of type %s)", exp_to_str(e), type_to_str(et))
  );
}

WrERs *wrERs_fromWrSRsFail (WrSRs *rs) {
  return new(TRUE, type_bool(), "", rs->code);
}

WrERs *wrERs_mk (Type *tp, char *fun_code, char *code) {
  return new(FALSE, tp, fun_code, code);
}
