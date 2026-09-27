// Copyright 08-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "model/uuqf.h"
#include "kut/DEFS.h"
#include "kut/arr.h"

Arr *libmkt_uuqf_refs_c (ModelParams *mpars) {
  int ndates = mpars->ndates;
  int ncos = mpars->ncos;
  double **closes = mpars->closes;
  double *pars = mpars->params;

  double start = pars[0];
  double up_start = 1 + start;
  double down_start = 1 - start;
  double gap = pars[0] * pars[1];
  double up_gap = 1 + gap;

  int is_solds[ncos];
  for (int i = 0; i < ncos; ++i) is_solds[i] = FALSE;
  double *closes0 = closes[0];
  double ref_us[ncos];
  double ref_qs[ncos];
  double top_qs[ncos];
  for (int i = 0; i < ncos; ++i) {
    double c = closes0[i];
    ref_us[i] = c * down_start;
    ref_qs[i] = c / up_gap;
    top_qs[i] = c * (1 + up_gap) / 2;
  }

  // <double>
  Arr *all_refs = arr_new();
  double *ref_qs2 = ATOMIC(sizeof(double) * ncos);
  memcpy(ref_qs2, ref_qs, sizeof(double) * ncos);
  arr_push(all_refs, ref_qs2);

  for (int idate = 1; idate < ndates; ++idate) {
    double *closes_r = closes[idate];
    double *prv_closes = closes[idate - 1];
    double *refs = ATOMIC(sizeof(double) * ncos);

    for (int ico = 0; ico < ncos; ++ico) {
      double c = closes_r[ico];
      double prv_c = prv_closes[ico];
      double rf_u = ref_us[ico];
      double rf_q = ref_qs[ico];
      double top_q = top_qs[ico];

      if (is_solds[ico]) {
        if (c > rf_u || c > rf_q) {
          is_solds[ico] = FALSE;
          ref_us[ico] = c * down_start;
          double r_q = c / up_gap;
          ref_qs[ico] = r_q;
          top_qs[ico] = c * (1 + up_gap) / 2;
          refs[ico] = r_q;
        } else {
          double rf_u0 = rf_u * c / prv_c;
          double rf_u2 = rf_u < rf_u0 ? rf_u : rf_u0;
          ref_us[ico] = rf_u2;

          while (c < top_q) {
            double r0 = rf_q / up_gap;
            rf_q = r0;
            double r = r0 / up_gap;
            top_q = (r + r / up_gap) / 2;
          }
          ref_qs[ico] = rf_q;
          top_qs[ico] = top_q;

          refs[ico] = rf_u2 < rf_q ? rf_u2 : rf_q;
        }
      } else {
        if (c < rf_u || c < rf_q) {
          is_solds[ico] = TRUE;
          ref_us[ico] = c * up_start;
          double r_q = c * up_gap;
          ref_qs[ico] = r_q;
          top_qs[ico] = (c + c / up_gap) / 2;
          refs[ico] = r_q;
        } else {
          double rf_u0 = rf_u * c / prv_c;
          double rf_u2 = rf_u > rf_u0 ? rf_u : rf_u0;
          ref_us[ico] = rf_u2;

          while (c > top_q) {
            double r0 = rf_q * up_gap;
            rf_q = r0;
            double r = r0 * up_gap;
            top_q = r * (1 + up_gap) / 2;
          }
          ref_qs[ico] = rf_q;
          top_qs[ico] = top_q;

          refs[ico] = rf_u2 > rf_q ? rf_u2 : rf_q;
        }
      }
    }
    arr_push(all_refs, refs);
  }

  return all_refs;
}

Exp *libmkt_uuqf_refs (Arr *params) {
  ModelParams *mpars = iface_model_params(params);
  return iface_model_return(mpars->ncos, libmkt_uuqf_refs_c(mpars));
}
