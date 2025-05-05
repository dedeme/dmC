// Copyright 02-Apr-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/built/mdTcp.h"
#include "comp/built.h"

WrERs *mdTcp_process (WrCtx *ctx, int ln, char *md, char *sym) {
  if (!strcmp(sym, "accept"))
    return built_mk_rs("[<tcpServer>|[. [<tcpConn>]s]]", md, sym);
  if (!strcmp(sym, "closeConnection")) return built_mk_rs("[<tcpConn>|]", md, sym);
  if (!strcmp(sym, "closeServer")) return built_mk_rs("[<tcpServer>|]", md, sym);
  if (!strcmp(sym, "dial")) return built_mk_rs("[s|[. [<tcpConn>]s]]", md, sym);
  if (!strcmp(sym, "read")) return built_mk_rs("[<tcpConn>ii|[. [<bytes>]s]]", md, sym);
  if (!strcmp(sym, "server")) return built_mk_rs("[ii|<tcpServer>]", md, sym);
  if (!strcmp(sym, "write")) return built_mk_rs("[<tcpConn><bytes>|s]", md, sym);

  return wrERs_fail(ctx, ln, str_f("Function '%s.%s' not found.", md, sym));
}
