// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "models.h"
#include "Model.h"
#include "models/Approx.h"

Arr *models_create_new() {
  // Arr[Model]
  Arr *r = arr_new((FPROC)model_free);

  arr_push(r, approx_new());

  return r;
}
