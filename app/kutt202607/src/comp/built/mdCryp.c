// Copyright 28-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/built/mdCryp.h"
#include "comp/built.h"

WrERs *mdCryp_process (WrCtx *ctx, int ln, char *md, char *sym) {
  if (!strcmp(sym, "decode")) return built_mk_rs("[ss|s]", md, sym);
  if (!strcmp(sym, "decodeBytes")) return built_mk_rs("[ss|<bytes>]", md, sym);
  if (!strcmp(sym, "encode")) return built_mk_rs("[ss|s]", md, sym);
  if (!strcmp(sym, "encodeBytes")) return built_mk_rs("[s<bytes>|s]", md, sym);
  if (!strcmp(sym, "genK")) return built_mk_rs("[i|s]", md, sym);
  if (!strcmp(sym, "key")) return built_mk_rs("[si|s]", md, sym);

  return wrERs_fail(ctx, ln, str_f("Function '%s.%s' not found.", md, sym));
}
