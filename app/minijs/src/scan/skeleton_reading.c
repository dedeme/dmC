// Copyright 27-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "scan/skeleton_reading.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "dmc/Opt.h"
#include "dmc/Tuples.h"
#include "scan/logic.h"
#include "scan/token.h"
#include "types/ast.h"
#include "types/SkeletonResult.h"
#include "scan/sk_syntax_fns.h"

SkeletonResult *skeleton_reading_run (Lchar *roots, Scanner *sc) {
  MskEntry *entries = mskEntry_new();

  // Blank*
  AstTp *p0 = ast_tp_new(opt_null(), (AstFn)token_blanks);

  // SkTopDeclaration
  AstTp *p100 = ast_tp_new(
    tp_new(roots, entries),
    (AstFn)sk_syntax_fns_SkTopDeclaration
  );
  AstArr *a10 = ast_arr_new();
  ast_arr_add(a10, p0);
  ast_arr_add(a10, p100);
  // SkTopDeclaration Blank*
  AstTp *p10 = ast_tp_new(a10, (AstFn)logic_and);
  // (SkTopDeclaration Blank*)*
  AstTp *p1 = ast_tp_new(p10, (AstFn)logic_one_or_more);

  // End
  AstTp *p2 = ast_tp_new(opt_null(), (AstFn)token_end);
  AstArr *a = ast_arr_new();
  ast_arr_add(a, p0);
  ast_arr_add(a, p1);
  ast_arr_add(a, p2);
  // Blank* (SkTopDeclaration Blank*)* End
  ScanResult *r = logic_and(a, sc);

  Skeleton *sk = skeleton_new(entries);
  return skeletonResult_new(scanResult_fails(r), sk);
}
