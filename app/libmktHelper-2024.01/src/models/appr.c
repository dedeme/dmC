// Copyright 29-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "models/appr.h"
#include "kut/DEFS.h"
#include "kut/js.h"

Arr *appr_calc_references (Arr *closes, Vec *params) {
  double start = params->vs[0];
  double up_start = 1 + start;
  double down_start = 1 - start;
  double incr = params->vs[1];

  Vec *cls0 = *arr_begin(closes);
  int ncos = cls0->size;

  // <Vec>
  Arr *all_refs = arr_new_bf(arr_size(closes));

  int *is_solds = ATOMIC(ncos * sizeof(int));
  double *refs = ATOMIC(ncos * sizeof(double));

  int *pis_solds = is_solds;
  double *pvcls = cls0->vs;
  double *prefs = refs;
  REPEAT(ncos) {
    *pis_solds++ = FALSE;
    *prefs++ = *pvcls++ * down_start;
  }_REPEAT

  arr_push(all_refs, vec_new(ncos, vec_dup(ncos, refs)));
  EACH(arr_drop(closes, 1), Vec, v) {
    double *closes_vs = v->vs;

    int *pis_solds = is_solds;
    double *pcloses_vs = closes_vs;
    double *prfs = refs;
    RANGE0(ico, ncos) {
      int is_sold = *pis_solds++;
      double c = *pcloses_vs++;
      double rf = *prfs++;

      if (is_sold) {
        if (c > rf) {
          is_solds[ico] = FALSE;
          refs[ico] = c * down_start;
        } else {
          refs[ico] = rf - (rf - c) * incr;
        }
      } else {
        if (c < rf) {
          is_solds[ico] = TRUE;
          refs[ico] = c * up_start;
        } else {
          refs[ico] = rf + (c - rf) * incr;
        }
      }
    }_RANGE

    arr_push(all_refs, vec_new(ncos, vec_dup(ncos, refs)));
  }_EACH

  return all_refs;
}

