// Copyright 08-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "model/qmob.h"
#include "kut/DEFS.h"
#include "kut/arr.h"

Arr *libmkt_qmob_refs_c (ModelParams *mpars) {
  int ndates = mpars->ndates;
  int ncos = mpars->ncos;
  double **closes = mpars->closes;
  double *pars = mpars->params;

  double gap = pars[0];
  double up_gap = 1 + gap;
  double down_gap = 1 - gap;

  int is_solds[ncos];
  for (int i = 0; i < ncos; ++i) is_solds[i] = FALSE;
  double *closes0 = closes[0];
  double refs[ncos];
  for (int i = 0; i < ncos; ++i) refs[i] = closes0[i] * down_gap;

  // <double>
  Arr *all_refs = arr_new();
  double *refs2 = ATOMIC(sizeof(double) * ncos);
  memcpy(refs2, refs, sizeof(double) * ncos);
  arr_push(all_refs, refs2);

  for (int idate = 1; idate < ndates; ++idate) {
    double *closes_r = closes[idate];

    for (int ico = 0; ico < ncos; ++ico) {
      double c = closes_r[ico];
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
    }
    refs2 = ATOMIC(sizeof(double) * ncos);
    memcpy(refs2, refs, sizeof(double) * ncos);
    arr_push(all_refs, refs2);
  }

  return all_refs;
}

Exp *libmkt_qmob_refs (Arr *params) {
  ModelParams *mpars = iface_model_params(params);
  return iface_model_return(mpars->ncos, libmkt_qmob_refs_c(mpars));
}
