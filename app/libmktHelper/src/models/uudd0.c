// Copyright 29-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "models/uudd0.h"
#include "kut/DEFS.h"
#include "kut/js.h"
#include "cts.h"

Arr *uudd0_calc_references (Arr *closes, Vec *params) {
  double start = params->vs[0];
  double up_start = 1 + start;
  double down_start = 1 - start;

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
        double rf2 = rf * c / prv_c;
        if (rf < rf2) rf2 = rf;
        if (c > rf2) {
          is_solds[ico] = FALSE;
          refs[ico] = c * down_start;
        } else {
          refs[ico] = rf2;
        }
      } else {
        double rf2 = rf * c / prv_c;
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

// Tp3<Vec, int, double>
Tp3 *uudd0_calc_real_references (
  int date_ix, double rref, Vec *closes, Vec *params
) {
  double start = params->vs[0];
  double up_start = 1 + start;
  double down_start = 1 - start;

  double *closes_vs = closes->vs;
  int closes_size = closes->size;

  double cl0 = *closes_vs;
  if (date_ix == -1) {
    date_ix = 0;
    rref = cl0 * down_start;
  }

  double *refs = ATOMIC(closes_size * sizeof(double));
  int *new_date_ix = ATOMIC(sizeof(int));
  *new_date_ix = 0;
  double *new_rref = ATOMIC(sizeof(double));
  *new_rref = 0;

  double *pcloses = closes_vs;
  double *prefs = refs;

  REPEAT(date_ix) {
    *prefs++ = *pcloses++ * down_start;
  }_REPEAT

  double cl = *pcloses;
  double rf = rref;
  *prefs = rf;
  int is_sold = rf > cl;

  int firstChange = TRUE;
  if (date_ix > cts_real_reference_date_ix) {
    *new_date_ix = date_ix;
    *new_rref = rref;
    firstChange = FALSE;
  }

  double prv_cl = cl;
  ++date_ix;
  ++pcloses;
  ++prefs;

  RANGE(i, date_ix, closes_size) {
    cl = *pcloses;
    if (is_sold) {
      double rf2 = rf * cl / prv_cl;
      if (rf < rf2) rf2 = rf;
      if (cl > rf2) {
        is_sold = FALSE;
        *prefs = cl * down_start;
        if (firstChange && i > cts_real_reference_date_ix) {
          *new_date_ix = i;
          *new_rref = *prefs;
          firstChange = FALSE;
        }
      } else {
        *prefs = rf2;
      }
    } else {
      double rf2 = rf * cl / prv_cl;
      if (rf > rf2) rf2 = rf;
      if (cl < rf2) {
        is_sold = TRUE;
        *prefs = cl * up_start;
        if (firstChange && i > cts_real_reference_date_ix) {
          *new_date_ix = i;
          *new_rref = *prefs;
          firstChange = FALSE;
        }
      } else {
        *prefs = rf2;
      }
    }

    prv_cl = cl;
    rf = *prefs;
    ++pcloses;
    ++prefs;
  }_RANGE

  return tp3_new(vec_new(closes_size, refs), new_date_ix, new_rref);
}
