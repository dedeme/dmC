// Copyright 28-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/built/mdB64.h"
#include "comp/built.h"

WrERs *mdDic_process (WrCtx *ctx, int ln, char *md, char *sym) {
  if (!strcmp(sym, "copy")) return built_mk_rs("[{A}|{A}]", md, sym);
  if (!strcmp(sym, "fromArr")) return built_mk_rs("[[[.sA]]|{A}]", md, sym);
  if (!strcmp(sym, "fromIter")) return built_mk_rs("[[![.sA]]|{A}]", md, sym);
  if (!strcmp(sym, "fromJs")) return built_mk_rs("[s[s|A]|{A}]", md, sym);
  if (!strcmp(sym, "get")) return built_mk_rs("[{A}s|[A]]", md, sym);
  if (!strcmp(sym, "hasKey")) return built_mk_rs("[{A}s|b]", md, sym);
  if (!strcmp(sym, "keys")) return built_mk_rs("[{A}|[s]]", md, sym);
  if (!strcmp(sym, "map")) return built_mk_rs("[{A}[A|B]|{B}]", md, sym);
  if (!strcmp(sym, "put")) return built_mk_rs("[{A}sA|]", md, sym);
  if (!strcmp(sym, "remove")) return built_mk_rs("[{A}s|]", md, sym);
  if (!strcmp(sym, "size")) return built_mk_rs("[{A}|i]", md, sym);
  if (!strcmp(sym, "toArr")) return built_mk_rs("[{A}|[[.sA]]]", md, sym);
  if (!strcmp(sym, "toIter")) return built_mk_rs("[{A}|[![.sA]]]", md, sym);
  if (!strcmp(sym, "toJs")) return built_mk_rs("[{A}[A|s]|s]", md, sym);
  if (!strcmp(sym, "values")) return built_mk_rs("[{A}|[A]]", md, sym);

  return wrERs_fail(ctx, ln, str_f("Function '%s.%s' not found.", md, sym));
}
