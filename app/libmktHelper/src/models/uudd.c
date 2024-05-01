// Copyright 29-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "models/uudd.h"
#include "kut/DEFS.h"
#include "kut/js.h"

Arr *uudd_calc_references (Arr *closes, Vec *params) {
  double start = params->vs[0];
  double up_start = 1 + start;
  double down_start = 1 - start;
  double speed = params->vs[1] * 10;

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
  double *prv_closes = cls0->vs;
  EACH(arr_drop(closes, 1), Vec, v) {
    double *closes_vs = v->vs;

    int *pis_solds = is_solds;
    double *pcloses_vs = closes_vs;
    double *pprv_closes = prv_closes;
    double *prfs = refs;
    RANGE0(ico, ncos) {
      int is_sold = *pis_solds++;
      double c = *pcloses_vs++;
      double prv_c = *pprv_closes++;
      double rf = *prfs++;

      if (is_sold) {
        double dif = c - prv_c;
        double rf2 = rf * (prv_c + dif * speed) / prv_c;
        if (rf < rf2) rf2 = rf;
        if (c > rf2) {
          is_solds[ico] = FALSE;
          refs[ico] = c * down_start;
        } else {
          refs[ico] = rf2;
        }
      } else {
        double dif = c - prv_c;
        double rf2 = rf * (prv_c + dif * speed) / prv_c;
        if (rf > rf2) rf2 = rf;
        if (c < rf2) {
          is_solds[ico] = TRUE;
          refs[ico] = c * up_start;
        } else {
          refs[ico] = rf2;
        }
      }
    }_RANGE

    arr_push(all_refs, vec_new(ncos, vec_dup(ncos, refs)));
    prv_closes = closes_vs;
  }_EACH

  return all_refs;
}
