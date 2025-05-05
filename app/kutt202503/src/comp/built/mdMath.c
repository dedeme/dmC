// Copyright 28-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/built/mdMath.h"
#include "comp/built.h"

WrERs *mdMath_process (WrCtx *ctx, int ln, char *md, char *sym) {
  if (!strcmp(sym, "abs")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "acos")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "acosh")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "asin")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "asinh")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "atan")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "atanh")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "ceil")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "cos")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "cosh")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "e")) return built_mk_rs("[|f]", md, sym);
  if (!strcmp(sym, "eq")) return built_mk_rs("[fff|b]", md, sym);
  if (!strcmp(sym, "exp")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "exp2")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "floor")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "fromEn")) return built_mk_rs("[s|[f]]", md, sym);
  if (!strcmp(sym, "fromHex")) return built_mk_rs("[s|[i]]", md, sym);
  if (!strcmp(sym, "fromIso")) return built_mk_rs("[s|[f]]", md, sym);
  if (!strcmp(sym, "ftoi")) return built_mk_rs("[f|i]", md, sym);
  if (!strcmp(sym, "ftos")) return built_mk_rs("[f|s]", md, sym);
  if (!strcmp(sym, "ftos2")) return built_mk_rs("[fi|s]", md, sym);
  if (!strcmp(sym, "isDigits")) return built_mk_rs("[s|b]", md, sym);
  if (!strcmp(sym, "itof")) return built_mk_rs("[i|f]", md, sym);
  if (!strcmp(sym, "itoh")) return built_mk_rs("[i|s]", md, sym);
  if (!strcmp(sym, "itos")) return built_mk_rs("[i|s]", md, sym);
  if (!strcmp(sym, "log")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "log10")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "log2")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "max")) return built_mk_rs("[ff|f]", md, sym);
  if (!strcmp(sym, "maxFloat")) return built_mk_rs("[|f]", md, sym);
  if (!strcmp(sym, "maxInt")) return built_mk_rs("[|i]", md, sym);
  if (!strcmp(sym, "min")) return built_mk_rs("[ff|f]", md, sym);
  if (!strcmp(sym, "minInt")) return built_mk_rs("[|i]", md, sym);
  if (!strcmp(sym, "pi")) return built_mk_rs("[|f]", md, sym);
  if (!strcmp(sym, "pow")) return built_mk_rs("[ff|f]", md, sym);
  if (!strcmp(sym, "pow10")) return built_mk_rs("[i|f]", md, sym);
  if (!strcmp(sym, "rnd")) return built_mk_rs("[|f]", md, sym);
  if (!strcmp(sym, "rndi")) return built_mk_rs("[i|i]", md, sym);
  if (!strcmp(sym, "round")) return built_mk_rs("[fi|f]", md, sym);
  if (!strcmp(sym, "sin")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "sinh")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "sqrt")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "stof")) return built_mk_rs("[s|[f]]", md, sym);
  if (!strcmp(sym, "stoi")) return built_mk_rs("[s|[i]]", md, sym);
  if (!strcmp(sym, "tan")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "tanh")) return built_mk_rs("[f|f]", md, sym);
  if (!strcmp(sym, "trunc")) return built_mk_rs("[f|f]", md, sym);

  return wrERs_fail(ctx, ln, str_f("Function '%s.%s' not found.", md, sym));
}
