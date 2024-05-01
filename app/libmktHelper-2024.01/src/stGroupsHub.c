// Copyright 30-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "strategiesHub.h"
#include "kut/DEFS.h"
#include "kut/str.h"
#include "kut/opt.h"
#include "kut/js.h"
#include "vec.h"
#include "modelsHub.h"
#include "strategies/openSimple.h"
#include "strategies/openSimple2.h"

char *stGroupsHub_run(Map *rq) {
  char *type = js_rs(opt_eget(map_get(rq, "type")));
  char *mdId = js_rs(opt_eget(map_get(rq, "modelId")));
  // <Vec>
  Arr *opens = vec_matrix_from_js(opt_eget(map_get(rq, "Opens")));
  // <Vec>
  Arr *closes = vec_matrix_from_js(opt_eget(map_get(rq, "Closes")));
  // <Vec>
  Arr *params = vec_matrix_from_js(opt_eget(map_get(rq, "Params")));
  int ndates = arr_size(opens);
  int ncos = ((Vec *)arr_peek(opens))->size;

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

  // <JS>
  Arr *sales = arr_new_bf(arr_size(params));
  // <JS>
  Arr *assets = arr_new_bf(arr_size(params));
  // <JS>
  Arr *accs = arr_new_bf(arr_size(params));
  // <JS>
  Arr *rf_assets = arr_new_bf(arr_size(params));

  if (str_eq(type, "open")) {
    EACH(params, Vec, pars) {
      // <Vec>
      Arr *refs = modelsHub_calc_references(mdId, closes, pars);

      double *rs = openSimple_calc(opens, closes, refs);
      arr_push(sales, js_wf(rs[0], 1));
      arr_push(assets, js_wf(rs[1], 3));
      arr_push(accs, js_wf(rs[2], 3));
      arr_push(rf_assets, js_wf(rs[3], 3));
    }_EACH

    return js_wo(map_from_array(arr_new_from(
      kv_new("Sales", js_wa(sales)),
      kv_new("Assets", js_wa(assets)),
      kv_new("Accs", js_wa(accs)),
      kv_new("RfAssets", js_wa(rf_assets)),
      NULL
    )));
  }

  if (str_eq(type, "open2")) {
    // <JS>
    Arr *profits = arr_new_bf(arr_size(params));
    // <JS>
    Arr *rf_profits = arr_new_bf(arr_size(params));

    EACH(params, Vec, pars) {
      // <Vec>
      Arr *refs = modelsHub_calc_references(mdId, closes, pars);

      double *rs = openSimple2_calc(opens, closes, refs);
      arr_push(sales, js_wf(rs[0], 1));
      arr_push(assets, js_wf(rs[1], 3));
      arr_push(accs, js_wf(rs[2], 3));
      arr_push(rf_assets, js_wf(rs[3], 3));
      arr_push(profits, js_wf(rs[4], 5));
      arr_push(rf_profits, js_wf(rs[5], 5));
    }_EACH

    return js_wo(map_from_array(arr_new_from(
      kv_new("Sales", js_wa(sales)),
      kv_new("Assets", js_wa(assets)),
      kv_new("Accs", js_wa(accs)),
      kv_new("RfAssets", js_wa(rf_assets)),
      kv_new("Profits", js_wa(profits)),
      kv_new("RfProfits", js_wa(rf_profits)),
      NULL
    )));
  }

  EXC_GENERIC(str_f("Strategy %s not found", type));
  return NULL; // Unreachable.

}
