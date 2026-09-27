// Copyright 07-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Interface c-kut.

#ifndef IFACE_H
  #define IFACE_H

#include "exp.h"

/// Parameters of market models maker.
typedef struct {
  int ndates;
  int ncos;
  double **closes;
  double *params;
} ModelParams;

/// Returns C-parameters from Arr<Exp<[[f.].]>, Exp<[f,]>>.
ModelParams *iface_model_params (Arr *params);

/// Returns return of maket models maker (Exp<[[f.].]>).
///   ncos: Companies number.
///   refs: References (Arr<double>).
Exp *iface_model_return (int ncos, Arr *refs);

/// Returns Exp<[f.]> from a C-array of double.
///   n     : Number of values.
///   values: volues to store.
Exp *iface_doubles_to_exp (int n, double *value);

#endif
