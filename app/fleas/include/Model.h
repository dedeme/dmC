// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef MODEL_H
  #define MODEL_H

#include "dmc/std.h"
#include "dmc/Darr.h"
#include "Flea.h"
#include "Order.h"

/// Function which returns the actual parameters of 'f'
typedef Darr *(*Fparams)(Flea *f);

/// Function which creates companies data.
///   params: Actual parameters (returned by 'model_params_new')
///   qnicks: Number of nicks-companies (columns of closes)
///   closes: Array of quotes. Rows are days, columns are nicks.
typedef Arr *(*Fcos)(Darr *params, int qnicks, double *closes);

/// Function which returns a market order.
///   params: Actual parameters (returned by model_params_new)
///   co: Company data. It is one of elements returned by 'model_cos_new'.
///       It will be modified.
///   q: Close quote.
typedef Order *(*Forder)(Darr *params, void *co, double q);

/// Function wich returns actual current reference.
///   params: Actual parameters (returned by model_params_new)
///   co: Company data. It is one of elements returned by 'model_cos_new'.
typedef double (*Fref)(Darr *params, void *co);

/*--*/

///
typedef struct model_Model Model;

///
Model *model_new(
  char *name,
  Varr *param_names,
  Js *param_jss,
  Fparams fparams_new,
  Fcos fcos_new,
  Forder forder_new,
  Fref fref
);

///
void model_free(Model *this);

/// Model name. Freed by this.
char *model_name(Model *this);

/// Names of model parameters. Freed by this.
Varr *model_param_names(Model *this);

/// Template to javascript. Freed by this.
Js *model_param_jss(Model *this);

/*--*/

/// Returns values of 'f' parameters adjusted with fparams_new
/// (see 'model_new').
Darr *model_params_new(Model *this, Flea *f);

/// Creates companies data.
///   this: Model
///   params: Actual parameters (returned by 'model_params_new')
///   qnicks: Number of nicks-companies (columns of closes)
///   closes: Array of quotes. Rows are days, columns are nicks.
Arr *model_cos_new(Model *this, Darr *params, int qnicks, double *closes);

/// Returns a market order.
///   this: Model
///   params: Actual parameters (returned by model_params_new)
///   co: Company data. It is one of elements returned by 'model_cos_new'.
///   q: Close quote.
Order *model_order_new(Model *this, Darr *params, void *co, double q);

/// Returns actual current reference.
///   this: Model
///   params: Actual parameters (returned by model_params_new)
///   co: Company data. It is one of elements returned by 'model_cos_new'.
double model_ref(Model *this, Darr *params, void *co);

#endif
