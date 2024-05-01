// Copyright 27-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "models/qmob.h"
#include "kut/DEFS.h"
#include "kut/js.h"

Arr *qmob_calc_references (Arr *closes, Vec *params) {
  double gap = params->vs[0];
  double up_gap = 1 + gap;
  double down_gap = 1 - gap;

  Vec *cls0 = *arr_begin(closes);
  int ncos = cls0->size;

  // <Vec>
  Arr *all_refs = arr_new_bf(arr_size(closes));

  double *refs = ATOMIC(ncos * sizeof(double));
  int *is_solds = ATOMIC(ncos * sizeof(int));

  double *pvcls = cls0->vs;
  double *prefs = refs;
  int *pis_solds = is_solds;
  REPEAT(ncos) {
    *prefs++ = *pvcls++ * down_gap;
    *pis_solds = FALSE;
  }_REPEAT

  EACH(closes, Vec, v) {
    double *vs = v->vs;
    RANGE0(ico, ncos) {
      double c = vs[ico];
      double rf = refs[ico];
      if (is_solds[ico]) {
        if (c > rf) {
          is_solds[ico] = FALSE;
          refs[ico] = c * down_gap;
        } else {
          double new_rf = c * up_gap;
          refs[ico] = new_rf > rf ? rf : new_rf;
        }
      } else {
        if (c < rf) {
          is_solds[ico] = TRUE;
          refs[ico] = c * up_gap;
        } else {
          double new_rf = c * down_gap;
          refs[ico] = new_rf < rf ? rf : new_rf;
        }
      }
    }_RANGE
    arr_push(all_refs, vec_new(ncos, vec_dup(ncos, refs)));
  }_EACH

  return all_refs;
}
