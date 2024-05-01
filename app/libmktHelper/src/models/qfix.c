// Copyright 27-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "models/qfix.h"
#include <math.h>
#include "kut/DEFS.h"
#include "kut/js.h"

Arr *qfix_calc_references (Arr *closes, Vec *params) {
  double jmp = params->vs[0] + 1;
  double lg_jmp = log(jmp);

  // FUNCTIONS -------------------------

  double down_gap (double q) {
    return pow(jmp, round(log(q)/lg_jmp) - 1);
  }

  double up_gap (double q) {
    return pow(jmp, round(log(q)/lg_jmp) + 1);
  }

  double down_gap2 (double q, double ref) {
    double ref2 = ref * jmp;
    return ref2 * sqrt(jmp) >= q ? ref : down_gap2(q, ref2);
  }

  double up_gap2 (double q, double ref) {
    double ref2 = ref / jmp;
    return ref2 / sqrt(jmp) <= q ? ref : up_gap2(q, ref2);
  }

  // END FUNCTIONS ---------------------

  Vec *cls0 = *arr_begin(closes);
  int ncos = cls0->size;

  // <Vec>
  Arr *all_refs = arr_new_bf(arr_size(closes));

  double *refs = ATOMIC(ncos * sizeof(double));

  double *pvcls = cls0->vs;
  double *prefs = refs;
  REPEAT(ncos) {
    *prefs++ = pow(jmp, round(log(*pvcls++)/lg_jmp) - 1) / jmp;
  }_REPEAT


  double *prv_closes = cls0->vs;
  arr_push(all_refs, vec_new(ncos, vec_dup(ncos, refs)));
  EACH(arr_drop(closes, 1), Vec, v) {
    double *cur_closes = v->vs;

    double *pprv_closes = prv_closes;
    double *pcur_closes = cur_closes;
    double *prfs = refs;

    RANGE0(ico, ncos) {
      double q0 = *pprv_closes++;
      double ref = *prfs++;
      double q = *pcur_closes++;

      if (q0 <= ref) {
        if (q < q0) {
          refs[ico] = up_gap2(q, ref);
        } else if (q > ref) {
          refs[ico] = down_gap(q);
        }
      } else {
        if (q > q0) {
          refs[ico] = down_gap2(q, ref);
        } else if (q <= ref) {
          refs[ico] = up_gap(q);
        }
      }
    }_RANGE
    prv_closes = cur_closes;
    arr_push(all_refs, vec_new(ncos, vec_dup(ncos, refs)));
  }_EACH

  return all_refs;
}
