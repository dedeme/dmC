// Copyright 01-Apr-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/built/mdThread.h"
#include "comp/built.h"

WrERs *mdThread_process (WrCtx *ctx, int ln, char *md, char *sym) {
  if (!strcmp(sym, "join")) return built_mk_rs("[<thread>|]", md, sym);
  if (!strcmp(sym, "run")) return built_mk_rs("[[|]|]", md, sym);
  if (!strcmp(sym, "start")) return built_mk_rs("[[|]|<thread>]", md, sym);
  if (!strcmp(sym, "sync")) return built_mk_rs("[[|]|]", md, sym);

  return wrERs_fail(ctx, ln, str_f("Function '%s.%s' not found.", md, sym));
}
