// Copyright 08-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "model/sprs.h"
#include "kut/DEFS.h"
#include "kut/arr.h"

Arr *libmkt_sprs_refs_c (ModelParams *mpars) {
  int ndates = mpars->ndates;
  int ncos = mpars->ncos;
  double **closes = mpars->closes;
  double *pars = mpars->params;

  double delta = pars[0];
  double up_delta = 1 + delta;
  double down_delta = 1 - delta;

  double tmps[ncos];
  for (int i = 0; i < ncos; ++i) tmps[i] = -1;
  int is_solds[ncos];
  for (int i = 0; i < ncos; ++i) is_solds[i] = FALSE;
  double *closes0 = closes[0];
  double maxs[ncos];
  double mins[ncos];
  for (int i = 0; i < ncos; ++i) {
    double c = closes0[i];
    maxs[i] = c;
    mins[i] = c * down_delta;
  }

  // <double>
  Arr *all_refs = arr_new();
  double *mins2 = ATOMIC(sizeof(double) * ncos);
  memcpy(mins2, mins, sizeof(double) * ncos);
  arr_push(all_refs, mins2);

  for (int idate = 1; idate < ndates; ++idate) {
    double *closes_r = closes[idate];
    double *refs = ATOMIC(sizeof(double) * ncos);

    for (int ico = 0; ico < ncos; ++ico) {
      double c = closes_r[ico];

      if (is_solds[ico]) {
        if (c > maxs[ico]) {
          is_solds[ico] = FALSE;
          maxs[ico] = c;
          tmps[ico] = -1;
          refs[ico] = mins[ico];
        } else {
          if (c < mins[ico]) {
            mins[ico] = c;
            if (tmps[ico] >= 0) {
              maxs[ico] = tmps[ico];
              tmps[ico] = -1;
            }
          } else if (
            (tmps[ico] < 0 && c > mins[ico] * up_delta) ||
            (tmps[ico] >= 0 && c > tmps[ico])
          ) {
            tmps[ico]= c;
          }
          refs[ico] = maxs[ico];
        }
      } else {
        if (c < mins[ico]) {
          is_solds[ico] = TRUE;
          mins[ico] = c;
          tmps[ico] = -1;
          refs[ico] = maxs[ico];
        } else {
          if (c > maxs[ico]) {
            maxs[ico] = c;
            if (tmps[ico] >= 0) {
              mins[ico] = tmps[ico];
              tmps[ico] = -1;
            }
          } else if (
            (tmps[ico] < 0 && c < maxs[ico] * down_delta) ||
            c < tmps[ico]
          ) {
            tmps[ico] = c;
          }
          refs[ico] = mins[ico];
        }
      }
    }
    arr_push(all_refs, refs);
  }

  return all_refs;
}

Exp *libmkt_sprs_refs (Arr *params) {
  ModelParams *mpars = iface_model_params(params);
  return iface_model_return(mpars->ncos, libmkt_sprs_refs_c(mpars));
}
