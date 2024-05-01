// Copyright 27-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "modelsHub.h"
#include "kut/DEFS.h"
#include "kut/str.h"
#include "kut/opt.h"
#include "kut/js.h"
#include "vec.h"
#include "models/appr.h"
#include "models/appr2.h"
#include "models/appr3.h"
#include "models/ea.h"
#include "models/ea2.h"
#include "models/ma.h"
#include "models/mm.h"
#include "models/qfix.h"
#include "models/qmob.h"
#include "models/uudd.h"
#include "models/uudd0.h"

// <Vec>
Arr *modelsHub_calc_references (char *mdId, Arr *closes, Vec *params) {
  if (str_eq(mdId, "APRX")) return appr_calc_references(closes, params);
  if (str_eq(mdId, "APRX2")) return appr2_calc_references(closes, params);
  if (str_eq(mdId, "APRX3")) return appr3_calc_references(closes, params);
  if (str_eq(mdId, "ME")) return ea_calc_references(closes, params);
  if (str_eq(mdId, "ME2")) return ea2_calc_references(closes, params);
  if (str_eq(mdId, "MM")) return ma_calc_references(closes, params);
  if (str_eq(mdId, "MX_MN")) return mm_calc_references(closes, params);
  if (str_eq(mdId, "QFIJO")) return qfix_calc_references(closes, params);
  if (str_eq(mdId, "QMOV")) return qmob_calc_references(closes, params);
  if (str_eq(mdId, "SS_BB")) return uudd_calc_references(closes, params);
  if (str_eq(mdId, "SSBB0")) return uudd0_calc_references(closes, params);
  EXC_GENERIC(str_f("Model %s not found", mdId));
  return NULL; // Unreachable
}

char *modelsHub_run(Map *rq) {
  char *mdId = js_rs(opt_eget(map_get(rq, "modelId")));
  // <Vec>
  Arr *closes = vec_matrix_from_js(opt_eget(map_get(rq, "Closes")));
  Vec *params = vec_from_js(opt_eget(map_get(rq, "Params")));

  return vec_matrix_to_js(
    modelsHub_calc_references(mdId, closes, params), 5
  );
}
