// Copyright 29-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "models/mm.h"
#include "kut/DEFS.h"
#include "kut/js.h"

Arr *mm_calc_references (Arr *closes, Vec *params) {
  int days = (int)(params->vs[0] + 0.4);
  double strip = params->vs[1];
  double strip_up = 1 + strip;
  double strip_down = 1 - strip;

  Vec *cls0 = *arr_begin(closes);
  int ncos = cls0->size;

  // <Vec>
  Arr *all_refs = arr_new_bf(arr_size(closes));

  int *is_solds = ATOMIC(ncos * sizeof(int));
  double *refs = ATOMIC(ncos * sizeof(double));

  int *pis_solds = is_solds;
  double *prefs = refs;
  REPEAT(ncos) {
    *pis_solds++ = FALSE;
    *prefs++ = 0;
  }_REPEAT

  Vec **pold_closes = (Vec **)arr_begin(closes);
  double *old_closes_vs = (*pold_closes++)->vs;
  EACH(closes, Vec, v) {
    double *closes_vs = v->vs;
    if (_i > days) old_closes_vs = (*pold_closes++)->vs;

    double *pcloses_vs = closes_vs;
    double *pold_closes_vs = old_closes_vs;
    RANGE0(ico, ncos) {
      double c = *pcloses_vs++;
      double old_c = *pold_closes_vs++;

      if (_i < days) {
        refs[ico] = c * strip_down;
      } else if (_i == days) {

        if (c <= old_c) {
          is_solds[ico] = TRUE;
          refs[ico] = old_c * strip_up;
        } else {
          refs[ico] = old_c * strip_down;
        }
      } else {
        double rf = refs[ico];

        if (is_solds[ico]) {
          if (c > rf) {
            is_solds[ico] = FALSE;
            refs[ico] = old_c * strip_down;
          } else {
            double new_rf = old_c * strip_up;
            if (new_rf < rf) refs[ico] = new_rf;
            else refs[ico] = rf;
          }
        } else {
          if (c < rf) {
            is_solds[ico] = TRUE;
            refs[ico] = old_c * strip_up;
          } else {
            double new_rf = old_c * strip_down;
            if (new_rf > rf) refs[ico] = new_rf;
            else refs[ico] = rf;
          }
        }
      }
    }_RANGE

    arr_push(all_refs, vec_new(ncos, vec_dup(ncos, refs)));
  }_EACH

  return all_refs;
}
