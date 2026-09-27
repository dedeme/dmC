// Copyright 08-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "model/ea2.h"
#include "kut/DEFS.h"
#include "kut/arr.h"

Arr *libmkt_ea2_refs_c (ModelParams *mpars) {
  int ndates = mpars->ndates;
  int ncos = mpars->ncos;
  double **closes = mpars->closes;
  double *pars = mpars->params;

  double days = pars[0];
  int idays = days;
  double strip = pars[1];
  double strip_up = 1 + strip;
  double strip_down = 1 - strip;

  int is_solds[ncos];
  for (int i = 0; i < ncos; ++i) is_solds[i] = FALSE;
  double *closes0 = closes[0];
  double refs[ncos];
  for (int i = 0; i < ncos; ++i) refs[i] = closes0[i] * strip_down;
  double avgs[ncos];
  memcpy(avgs, closes0, sizeof(double) * ncos);

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
      double avg = avgs[ico];
      double new_avg = idate < idays
        ? avg + (c - avg) / idate
        : avg + (c - avg) / days
      ;
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
    refs2 = ATOMIC(sizeof(double) * ncos);
    memcpy(refs2, refs, sizeof(double) * ncos);
    arr_push(all_refs, refs2);
  }

  return all_refs;
}

Exp *libmkt_ea2_refs (Arr *params) {
  ModelParams *mpars = iface_model_params(params);
  return iface_model_return(mpars->ncos, libmkt_ea2_refs_c(mpars));
}
