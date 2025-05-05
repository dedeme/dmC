// Copyright 31-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/built/mdTime.h"
#include "comp/built.h"

WrERs *mdTime_process (WrCtx *ctx, int ln, char *md, char *sym) {
  if (!strcmp(sym, "addDays")) return built_mk_rs("[ii|i]", md, sym);
  if (!strcmp(sym, "day")) return built_mk_rs("[i|i]", md, sym);
  if (!strcmp(sym, "dfDays")) return built_mk_rs("[ii|i]", md, sym);
  if (!strcmp(sym, "eqDay")) return built_mk_rs("[ii|b]", md, sym);
  if (!strcmp(sym, "fmt")) return built_mk_rs("[is|s]", md, sym);
  if (!strcmp(sym, "fromClock")) return built_mk_rs("[is|[i]]", md, sym);
  if (!strcmp(sym, "fromEn")) return built_mk_rs("[ss|[i]]", md, sym);
  if (!strcmp(sym, "fromIso")) return built_mk_rs("[ss|[i]]", md, sym);
  if (!strcmp(sym, "fromStr")) return built_mk_rs("[s|[i]]", md, sym);
  if (!strcmp(sym, "hour")) return built_mk_rs("[i|i]", md, sym);
  if (!strcmp(sym, "millisecond")) return built_mk_rs("[i|i]", md, sym);
  if (!strcmp(sym, "minute")) return built_mk_rs("[i|i]", md, sym);
  if (!strcmp(sym, "month")) return built_mk_rs("[i|i]", md, sym);
  if (!strcmp(sym, "new")) return built_mk_rs("[iiiiii|i]", md, sym);
  if (!strcmp(sym, "newDate")) return built_mk_rs("[iii|i]", md, sym);
  if (!strcmp(sym, "now")) return built_mk_rs("[|i]", md, sym);
  if (!strcmp(sym, "second")) return built_mk_rs("[i|i]", md, sym);
  if (!strcmp(sym, "toEn")) return built_mk_rs("[i|s]", md, sym);
  if (!strcmp(sym, "toIso")) return built_mk_rs("[i|s]", md, sym);
  if (!strcmp(sym, "toStr")) return built_mk_rs("[i|s]", md, sym);
  if (!strcmp(sym, "weekday")) return built_mk_rs("[i|i]", md, sym);
  if (!strcmp(sym, "year")) return built_mk_rs("[i|i]", md, sym);
  if (!strcmp(sym, "yearDay")) return built_mk_rs("[i|i]", md, sym);

  return wrERs_fail(ctx, ln, str_f("Function '%s.%s' not found.", md, sym));
}

