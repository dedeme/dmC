// Copyright 08-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "model/qfix.h"
#include <math.h>
#include "kut/DEFS.h"
#include "kut/arr.h"
#include "kut/math.h"

static double down_gap (double q, double jmp, double lg_jmp) {
  return pow(jmp, math_round(log(q)/lg_jmp, 0) - 1.0);
}

static double up_gap (double q, double jmp, double lg_jmp) {
  return pow(jmp, math_round(log(q)/lg_jmp, 0) + 1.0);
}

static double down_gap2 (double q, double ref, double jmp) {
  double ref2 = ref * jmp;
  return ref2 * sqrt(jmp) >= q ? ref : down_gap2(q, ref2, jmp);
}

static double up_gap2 (double q, double ref, double jmp) {
  double ref2 = ref / jmp;
  return ref2 / sqrt(jmp) <= q ? ref : up_gap2(q, ref2, jmp);
}

Arr *libmkt_qfix_refs_c (ModelParams *mpars) {
  int ndates = mpars->ndates;
  int ncos = mpars->ncos;
  double **closes = mpars->closes;
  double *pars = mpars->params;

  double jmp = pars[0] + 1;
  double lg_jmp = log(jmp);

  double *closes0 = closes[0];
  double refs[ncos];
  for (int i = 0; i < ncos; ++i)
    refs[i] = pow(jmp, math_round(log(closes0[i])/lg_jmp, 0) - 2);

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
      double c0 = prv_closes[ico];
      double rf = refs[ico];

      if (c0 <= rf) {
        if (c < c0) {
          refs[ico] = up_gap2(c, rf, jmp);
        } else if (c > rf) {
          refs[ico] = down_gap(c, jmp, lg_jmp);
        }
      } else {
        if (c > c0) {
          refs[ico] = down_gap2(c, rf, jmp);
        } else if (c < rf) {
          refs[ico] = up_gap(c, jmp, lg_jmp);
        }
      }
    }
    refs2 = ATOMIC(sizeof(double) * ncos);
    memcpy(refs2, refs, sizeof(double) * ncos);
    arr_push(all_refs, refs2);
  }

  return all_refs;
}

Exp *libmkt_qfix_refs (Arr *params) {
  ModelParams *mpars = iface_model_params(params);
  return iface_model_return(mpars->ncos, libmkt_qfix_refs_c(mpars));
}
