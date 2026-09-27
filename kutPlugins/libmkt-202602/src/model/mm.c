// Copyright 08-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "model/mm.h"
#include "kut/DEFS.h"
#include "kut/arr.h"

Arr *libmkt_mm_refs_c (ModelParams *mpars) {
  int ndates = mpars->ndates;
  int ncos = mpars->ncos;
  double **closes = mpars->closes;
  double *pars = mpars->params;

  int idays = pars[0];
  double strip = pars[1];
  double strip_up = 1 + strip;
  double strip_down = 1 - strip;

  int is_solds[ncos];
  for (int i = 0; i < ncos; ++i) is_solds[i] = FALSE;
  double *closes0 = closes[0];
  double refs[ncos];
  for (int i = 0; i < ncos; ++i) refs[i] = closes0[i] * strip_down;

  // <double>
  Arr *all_refs = arr_new();
  double *refs2 = ATOMIC(sizeof(double) * ncos);
  memcpy(refs2, refs, sizeof(double) * ncos);
  arr_push(all_refs, refs2);

  for (int idate = 1; idate < ndates; ++idate) {
    double *closes_r = closes[idate];
    double *old_closes = idate >= idays ? closes[idate-idays] : closes0;

    for (int ico = 0; ico < ncos; ++ico) {
      double c = closes_r[ico];
      double old_c = old_closes[ico];
      double rf = refs[ico];

      if (is_solds[ico]) {
        if (c > rf) {
          is_solds[ico] = FALSE;
          refs[ico] = old_c * strip_down;
        } else {
          double rf0 = old_c * strip_up;
          double rf2 = (rf0 < rf) ? rf0 : rf;
          refs[ico] = rf2;
        }
      } else {
        if (c < rf) {
          is_solds[ico] = TRUE;
          refs[ico] = old_c * strip_up;
        } else {
          double rf0 = old_c * strip_down;
          double rf2 = (rf0 > rf) ? rf0 : rf;
          refs[ico] = rf2;
        }
      }
    }
    refs2 = ATOMIC(sizeof(double) * ncos);
    memcpy(refs2, refs, sizeof(double) * ncos);
    arr_push(all_refs, refs2);
  }

  return all_refs;
}

Exp *libmkt_mm_refs (Arr *params) {
  ModelParams *mpars = iface_model_params(params);
  return iface_model_return(mpars->ncos, libmkt_mm_refs_c(mpars));
}
