// Copyright 28-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/built/mdSys.h"
#include "comp/built.h"

WrERs *mdSys_process (WrCtx *ctx, int ln, char *md, char *sym) {
  if (!strcmp(sym, "args")) return built_mk_rs("[|[s]]", md, sym);
  if (!strcmp(sym, "cmd")) return built_mk_rs("[s[s]|[. [s]s]]", md, sym);
  if (!strcmp(sym, "cmd2")) return built_mk_rs("[s[s]|[. ss]]", md, sym);
  if (!strcmp(sym, "compilerRoot")) return built_mk_rs("[|s]", md, sym);
  if (!strcmp(sym, "environ")) return built_mk_rs("[|{s}]", md, sym);
  if (!strcmp(sym, "exit")) return built_mk_rs("[i|]", md, sym);
  if (!strcmp(sym, "getLocale")) return built_mk_rs("[|s]", md, sym);
  if (!strcmp(sym, "home")) return built_mk_rs("[|s]", md, sym);
  if (!strcmp(sym, "mainPath")) return built_mk_rs("[|s]", md, sym);
  if (!strcmp(sym, "pid")) return built_mk_rs("[|i]", md, sym);
  if (!strcmp(sym, "printError")) return built_mk_rs("[s|]", md, sym);
  if (!strcmp(sym, "readLine")) return built_mk_rs("[|s]", md, sym);
  if (!strcmp(sym, "setLocale")) return built_mk_rs("[s|]", md, sym);
  if (!strcmp(sym, "sleep")) return built_mk_rs("[i|]", md, sym);
  if (!strcmp(sym, "user")) return built_mk_rs("[|s]", md, sym);

  if (
    !strcmp(sym, "print") ||
    !strcmp(sym, "println") ||
    !strcmp(sym, "test") ||
    !strcmp(sym, "toStr") ||
    !strcmp(sym, "type") ||
    !strcmp(sym, "typeExpand") ||
    !strcmp(sym, "used")
  )
    return wrERs_fail(ctx, ln, str_f(
      "Function '%s.%s' only can be used in function call.", md, sym
    ));

  return wrERs_fail(ctx, ln, str_f("Function '%s.%s' not found.", md, sym));
}
