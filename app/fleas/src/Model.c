// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Model.h"

/* .
Model
  #Model name. Freed by this.
  name: char *
  #Names of model parameters. Freed by this.
  param_names: Varr *
  #Template to javascript. Freed by this.
  param_jss: Js *: _s
  -fparams_new: Fparams: _i: _i
  -fcos_new: Fcos: _i: _i
  -forder_new: Forder: _i: _i
  -fref: Fref: _i: _i
*/

/*--*/

struct model_Model {
  char *name;
  Varr *param_names;
  Js *param_jss;
  Fparams fparams_new;
  Fcos fcos_new;
  Forder forder_new;
  Fref fref;
};

Model *model_new(
  char *name,
  Varr *param_names,
  Js *param_jss,
  Fparams fparams_new,
  Fcos fcos_new,
  Forder forder_new,
  Fref fref
) {
  Model *this = malloc(sizeof(Model));
  this->name = name;
  this->param_names = param_names;
  this->param_jss = param_jss;
  this->fparams_new = fparams_new;
  this->fcos_new = fcos_new;
  this->forder_new = forder_new;
  this->fref = fref;
  return this;
}

void model_free(Model *this) {
  free(this->name);
  varr_free(this->param_names);
  free(this->param_jss);
  free(this);
};

char *model_name(Model *this) {
  return this->name;
}

Varr *model_param_names(Model *this) {
  return this->param_names;
}

Js *model_param_jss(Model *this) {
  return this->param_jss;
}

/*--*/

Darr *model_params_new(Model *this, Flea *f) {
  return this->fparams_new(f);
}

Arr *model_cos_new(Model *this, Darr *params, int qnicks, double *closes) {
  return this->fcos_new(params, qnicks, closes);
}

Order *model_order_new(Model *this, Darr *params, void *co, double q) {
  return this->forder_new(params, co, q);
}

double model_ref(Model *this, Darr *params, void *co) {
  return this->fref(params, co);
}
