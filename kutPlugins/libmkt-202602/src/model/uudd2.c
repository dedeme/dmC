// Copyright 08-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "model/uudd2.h"
#include "kut/DEFS.h"
#include "kut/arr.h"

Arr *libmkt_uudd2_refs_c (ModelParams *mpars) {
  int ndates = mpars->ndates;
  int ncos = mpars->ncos;
  double **closes = mpars->closes;
  double *pars = mpars->params;

  double start_sale = pars[0];
  double start_buy = pars[1];
  double up_start = 1 + start_sale;
  double down_start = 1 - start_buy;

  int is_solds[ncos];
  for (int i = 0; i < ncos; ++i) is_solds[i] = FALSE;
  double *closes0 = closes[0];
  double refs[ncos];
  for (int i = 0; i < ncos; ++i) refs[i] = closes0[i] * down_start;

  // <double>
  Arr *all_refs = arr_new();
  double *refs2 = ATOMIC(sizeof(double) * ncos);
  memcpy(refs2, refs, sizeof(double) * ncos);
  arr_push(all_refs, refs2);

  for (int idate = 1; idate < ndates; ++idate) {
    double *closes_r = closes[idate];
    double *prv_closes = closes[idate - 1];

    for (int ico = 0; ico < ncos; ++ico) {
      double c = closes_r[ico];
      double prv_c = prv_closes[ico];
      double rf = refs[ico];

      if (is_solds[ico]) {
        if (c > rf) {
          is_solds[ico] = FALSE;
          refs[ico] = c * down_start;
        } else {
          double dif = c - prv_c;
          double rf0 = rf * (prv_c + dif) / prv_c;
          double rf2 = rf < rf0 ? rf : rf0;
          refs[ico] = rf2;
        }
      } else {
        if (c < rf) {
          is_solds[ico] = TRUE;
          refs[ico] = c * up_start;
        } else {
          double dif = c - prv_c;
          double rf0 = rf * (prv_c + dif) / prv_c;
          double rf2 = rf > rf0 ? rf : rf0;
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

Exp *libmkt_uudd2_refs (Arr *params) {
  ModelParams *mpars = iface_model_params(params);
  return iface_model_return(mpars->ncos, libmkt_uudd2_refs_c(mpars));
}
