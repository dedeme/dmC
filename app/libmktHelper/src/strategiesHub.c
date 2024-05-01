// Copyright 27-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "strategiesHub.h"
#include "kut/DEFS.h"
#include "kut/str.h"
#include "kut/opt.h"
#include "kut/js.h"
#include "vec.h"
#include "modelsHub.h"
#include "strategies/open.h"
#include "strategies/openSimple.h"
#include "strategies/openSimple2.h"

char *strategiesHub_run(Map *rq) {
  char *type = js_rs(opt_eget(map_get(rq, "type")));
  char *mdId = js_rs(opt_eget(map_get(rq, "modelId")));
  // <char>
  Arr *dates = arr_map(js_ra(opt_eget(map_get(rq, "Dates"))), (FMAP) js_rs);
  int ndates = arr_size(dates);
  // <char>
  Arr *cos = arr_map(js_ra(opt_eget(map_get(rq, "Cos"))), (FMAP) js_rs);
  int ncos = arr_size(cos);
  // <Vec>
  Arr *opens = vec_matrix_from_js(opt_eget(map_get(rq, "Opens")));
  // <Vec>
  Arr *closes = vec_matrix_from_js(opt_eget(map_get(rq, "Closes")));
  Vec *params = vec_from_js(opt_eget(map_get(rq, "Params")));

  if (arr_size(opens) != ndates)
    EXC_ILLEGAL_ARGUMENT(
      "Wrong dates number of opens",
      str_f("%d", ndates), str_f("%d", arr_size(opens))
    );
  if (arr_size(closes) != ndates)
    EXC_ILLEGAL_ARGUMENT(
      "Wrong dates number of closes",
      str_f("%d", ndates), str_f("%d", arr_size(closes))
    );

  Vec *v = (Vec *)*arr_begin(opens);
  if (v->size != ncos)
    EXC_ILLEGAL_ARGUMENT(
      "Wrong companies number in opens",
      str_f("%d", ncos), str_f("%d", v->size)
    );
  v = (Vec *)*arr_begin(closes);
  if (v->size != ncos)
    EXC_ILLEGAL_ARGUMENT(
      "Wrong companies number in closes",
      str_f("%d", ncos), str_f("%d", v->size)
    );

  // <Vec>
  Arr *refs = modelsHub_calc_references(mdId, closes, params);

  if (str_eq(type, "open"))
    return open_process(dates, cos, opens, closes, refs);
  if (str_eq(type, "openSimple"))
    return openSimple_process(opens, closes, refs);
  if (str_eq(type, "openSimple2"))
    return openSimple2_process(opens, closes, refs);
  else EXC_GENERIC(str_f("Strategy %s not found", type));

  return NULL; // Unreachable
}
