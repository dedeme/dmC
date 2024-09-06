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
Arr *modelsHub_calc_references (char *md_id, Arr *closes, Vec *params) {
  if (str_eq(md_id, "APRX")) return appr_calc_references(closes, params);
  if (str_eq(md_id, "APRX2")) return appr2_calc_references(closes, params);
  if (str_eq(md_id, "APRX3")) return appr3_calc_references(closes, params);
  if (str_eq(md_id, "ME")) return ea_calc_references(closes, params);
  if (str_eq(md_id, "ME2")) return ea2_calc_references(closes, params);
  if (str_eq(md_id, "MM")) return ma_calc_references(closes, params);
  if (str_eq(md_id, "MX_MN")) return mm_calc_references(closes, params);
  if (str_eq(md_id, "QFIJO")) return qfix_calc_references(closes, params);
  if (str_eq(md_id, "QMOV")) return qmob_calc_references(closes, params);
  if (str_eq(md_id, "SS_BB")) return uudd_calc_references(closes, params);
  if (str_eq(md_id, "SSBB0")) return uudd0_calc_references(closes, params);
  EXC_GENERIC(str_f("Model %s not found", md_id));
  return NULL; // Unreachable
}

char *modelsHub_run(Map *rq) {
  char *md_id = js_rs(opt_eget(map_get(rq, "modelId")));
  // <Vec>
  Arr *closes = vec_matrix_from_js(opt_eget(map_get(rq, "Closes")));
  Vec *params = vec_from_js(opt_eget(map_get(rq, "Params")));

  return vec_matrix_to_js(
    modelsHub_calc_references(md_id, closes, params), 5
  );
}

// Tp3<Vec, int, double>
Tp3 *calc_real_references (
  char *md_id, int dateIx, double rref, Vec *closes, Vec *params
) {
  if (str_eq(md_id, "SSBB0")) return uudd0_calc_real_references(
      dateIx, rref, closes, params
    );
  EXC_GENERIC(str_f("Model %s not found", md_id));
  return NULL; // Unreachable
}

char *modelsHub_run_real(Map *rq) {
  char *md_id = js_rs(opt_eget(map_get(rq, "modelId")));
  int dateIx = js_ri(opt_eget(map_get(rq, "dateIx")));
  double rref = js_rd(opt_eget(map_get(rq, "rref")));
  Vec *closes = vec_from_js(opt_eget(map_get(rq, "Closes")));
  Vec *params = vec_from_js(opt_eget(map_get(rq, "Params")));
  // Tp3<Vec, int, double>
  Tp3 *tp = calc_real_references(md_id, dateIx, rref, closes, params);
  return js_wa(arr_new_from(
    vec_to_js(tp3_e1(tp), 5),
    js_wi(*((int *)tp3_e2(tp))),
    js_wf(*((double *)tp3_e3(tp)), 5),
    NULL
  ));
}
