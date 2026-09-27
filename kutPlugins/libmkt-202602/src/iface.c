// Copyright 07-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "iface.h"
#include "kut/DEFS.h"
#include "cquotes.h"
#include "DEFS.h"

// params is Arr<Exp>
ModelParams *iface_model_params (Arr *params) {
  Exp **aparams = (Exp**)arr_begin(params);

  Quotes *qts = exp_get_object("cquotes", aparams[0]);

  // <Exp>
  Arr *apars = exp_get_array(aparams[1]);
  int npars = arr_size(apars);
  Exp **epars = (Exp **)arr_begin(apars);
  double *pars = ATOMIC(sizeof(double) * npars);
  for (int i = 0; i < npars; ++i) pars[i] = exp_get_float(epars[i]);

  ModelParams *this = MALLOC(ModelParams);
  this->ndates = libmkt_HISTORIC_QUOTES;
  this->ncos = qts->ncos;
  this->closes = qts->closes;
  this->params = pars;
  return this;
}

// refs is Arr<double>
Exp *iface_model_return (int ncos, Arr *refs) {
  // <Exp>
  Arr *r = arr_new_bf(libmkt_HISTORIC_QUOTES);
  EACH (refs, double, row) {
    // <Exp>
    Arr *rrow = arr_new();
    for (int i = 0; i < ncos; ++i)
      arr_push(rrow, exp_float(row[i]));
    arr_push(r, exp_array(rrow));
  }_EACH
  return exp_array(r);
}

Exp *iface_doubles_to_exp (int n, double *value) {
  // <Exp>
  Arr *r = arr_new();
  for (int i = 0; i < n; ++i) arr_push(r, exp_float(value[i]));
  return exp_array(r);
}
