// Copyright 25-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/built.h"
#include "comp/built/mdArr.h"
#include "comp/built/mdB64.h"
#include "comp/built/mdBytes.h"
#include "comp/built/mdCryp.h"
#include "comp/built/mdDic.h"
#include "comp/built/mdFile.h"
#include "comp/built/mdIter.h"
#include "comp/built/mdJs.h"
#include "comp/built/mdMath.h"
#include "comp/built/mdRegex.h"
#include "comp/built/mdStr.h"
#include "comp/built/mdSys.h"
#include "comp/built/mdTcp.h"
#include "comp/built/mdThread.h"
#include "comp/built/mdTime.h"
#include "fns.h"

WrERs *built_get (WrCtx *ctx, int ln, char *md, char *sym) {
  void *fn = NULL;
  if (str_eq(md, "arr")) fn = mdArr_process;
  else if (str_eq(md, "b64")) fn = mdB64_process;
  else if (str_eq(md, "bytes")) fn = mdBytes_process;
  else if (str_eq(md, "cryp")) fn = mdCryp_process;
  else if (str_eq(md, "dic")) fn = mdDic_process;
  else if (str_eq(md, "file")) fn = mdFile_process;
  else if (str_eq(md, "iter")) fn = mdIter_process;
  else if (str_eq(md, "js")) fn = mdJs_process;
  else if (str_eq(md, "math")) fn = mdMath_process;
  else if (str_eq(md, "regex")) fn = mdRegex_process;
  else if (str_eq(md, "str")) fn = mdStr_process;
  else if (str_eq(md, "sys")) fn = mdSys_process;
  else if (str_eq(md, "tcp")) fn = mdTcp_process;
  else if (str_eq(md, "thread")) fn = mdThread_process;
  else if (str_eq(md, "time")) fn = mdTime_process;

  if (!fn)
    return wrERs_fail(ctx, ln, str_f("Module '%s' not found.", md));
  return ((WrERs *(*)(WrCtx *, int, char *, char *))fn)(ctx, ln, md, sym);
}

WrERs *built_mk_rs (char *tp, char *md, char *sym) {
  Type *t = rs_get(type_read(tp));
  if (!t) EXC_KUTT(str_f("Type '%s' is not valid"));
  return wrERs_mk(t, "", str_f("((Val)(void *)__%s_%s)", md, sym));
}

WrERs *built_mk_rs_ex (char *tp, WrCtx *ctx, int ln, char *md, char *sym) {
  Type *t = rs_get(type_read(tp));
  if (!t) EXC_KUTT(str_f("Type '%s' is not valid"));
  char *id = fns_next_id(ctx->md_id);
  int npars = type_fnpars(t);
  // Arr<char>
  Arr *pars = arr_new();
  for (int i = 0; i < npars; ++i) arr_push(pars, str_f("Val v%d", i));

  Arr *vals = arr_new_from(fns_mk_pos(ctx->md_id, ln), NULL);
  for (int i = 0; i < npars; ++i) arr_push(vals, str_f("v%d", i));

  return wrERs_mk(t,
    str_f(
      "%s%s(%s){\n"
      "__%s_%s(%s);\n"
      "}",
      opt_get(type_frtype(t)) ? "Val " : "void ", id, arr_cjoin(pars, ','),
      md, sym, arr_cjoin(vals, ',')
    ),
    str_f("((Val)(void *)%s)", id)
  );
}
