// Copyright 01-Apr-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/built/mdFile.h"
#include "comp/built.h"

WrERs *mdFile_process (WrCtx *ctx, int ln, char *md, char *sym) {
  if (!strcmp(sym, "aopen")) return built_mk_rs("[s|<file>]", md, sym);
  if (!strcmp(sym, "base")) return built_mk_rs("[s|s]", md, sym);
  if (!strcmp(sym, "canonical")) return built_mk_rs_ex("[s|s]", ctx, ln, md, sym);
  if (!strcmp(sym, "cat")) return built_mk_rs("[s[s]|s]", md, sym);
  if (!strcmp(sym, "cd")) return built_mk_rs("[s|]", md, sym);
  if (!strcmp(sym, "clean")) return built_mk_rs("[s|s]", md, sym);
  if (!strcmp(sym, "close")) return built_mk_rs("[<file>|]", md, sym);
  if (!strcmp(sym, "copy")) return built_mk_rs_ex("[ss|]", ctx, ln, md, sym);
  if (!strcmp(sym, "del")) return built_mk_rs("[s|]", md, sym);
  if (!strcmp(sym, "dir")) return built_mk_rs("[s|[s]]", md, sym);
  if (!strcmp(sym, "exists")) return built_mk_rs("[s|b]", md, sym);
  if (!strcmp(sym, "extension")) return built_mk_rs("[s|s]", md, sym);
  if (!strcmp(sym, "isDirectory")) return built_mk_rs("[s|b]", md, sym);
  if (!strcmp(sym, "isLink")) return built_mk_rs("[s|b]", md, sym);
  if (!strcmp(sym, "isRegular")) return built_mk_rs("[s|b]", md, sym);
  if (!strcmp(sym, "mkdir")) return built_mk_rs("[s|]", md, sym);
  if (!strcmp(sym, "mklink")) return built_mk_rs("[ss|]", md, sym);
  if (!strcmp(sym, "parent")) return built_mk_rs("[s|s]", md, sym);
  if (!strcmp(sym, "read")) return built_mk_rs("[s|s]", md, sym);
  if (!strcmp(sym, "readBin")) return built_mk_rs("[<file>i|<bytes>]", md, sym);
  if (!strcmp(sym, "readLine")) return built_mk_rs("[<file>|[s]]", md, sym);
  if (!strcmp(sym, "rename")) return built_mk_rs("[ss|]", md, sym);
  if (!strcmp(sym, "ropen")) return built_mk_rs("[s|<file>]", md, sym);
  if (!strcmp(sym, "size")) return built_mk_rs("[s|i]", md, sym);
  if (!strcmp(sym, "tm")) return built_mk_rs("[s|i]", md, sym);
  if (!strcmp(sym, "tmp")) return built_mk_rs("[ss|s]", md, sym);
  if (!strcmp(sym, "wd")) return built_mk_rs("[|s]", md, sym);
  if (!strcmp(sym, "wopen")) return built_mk_rs("[s|<file>]", md, sym);
  if (!strcmp(sym, "write")) return built_mk_rs("[ss|]", md, sym);
  if (!strcmp(sym, "writeBin")) return built_mk_rs("[<file><bytes>|]", md, sym);
  if (!strcmp(sym, "writeText")) return built_mk_rs("[<file>s|]", md, sym);

  return wrERs_fail(ctx, ln, str_f("Function '%s.%s' not found.", md, sym));
}
