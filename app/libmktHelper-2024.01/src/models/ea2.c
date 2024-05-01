// Copyright 29-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "models/ea2.h"
#include "kut/DEFS.h"
#include "kut/js.h"

Arr *ea2_calc_references (Arr *closes, Vec *params) {
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
  double *avgs = ATOMIC(ncos * sizeof(double));

  int *pis_solds = is_solds;
  double *prefs = refs;
  double *pavgs = avgs;
  REPEAT(ncos) {
    *pis_solds++ = FALSE;
    *prefs++ = 0;
    *pavgs++ = 0;
  }_REPEAT

  EACH(closes, Vec, v) {
    double *closes_vs = v->vs;

    double *pcloses_vs = closes_vs;
    RANGE0(ico, ncos) {
      double c = *pcloses_vs++;
      if (_i < days) {
        avgs[ico] += c;
        refs[ico] = c * strip_down;
      } else if (_i == days) {
        double avg = avgs[ico] / days;
        double new_avg = avg + (c - avg) / days;
        avgs[ico] = new_avg;

        if (c <= new_avg) {
          is_solds[ico] = TRUE;
          refs[ico] = new_avg * strip_up;
        } else {
          refs[ico] = new_avg * strip_down;
        }
      } else {
        double rf = refs[ico];
        double avg = avgs[ico];
        double new_avg = avg + (c - avg) / days;
        avgs[ico] = new_avg;

        if (is_solds[ico]) {
          if (c > rf) {
            is_solds[ico] = FALSE;
            refs[ico] = new_avg * strip_down;
          } else {
            double new_rf = new_avg * strip_up;
            if (new_rf < rf) refs[ico] = new_rf;
            else refs[ico] = rf;
          }
        } else {
          if (c < rf) {
            is_solds[ico] = TRUE;
            refs[ico] = new_avg * strip_up;
          } else {
            double new_rf = new_avg * strip_down;
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

