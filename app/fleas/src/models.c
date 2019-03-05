// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "models.h"
#include "Model.h"
#include "models/Approx.h"
#include "models/Approx3a.h"
#include "models/Approx3b.h"
#include "models/Approx2.h"
#include "models/MinMax.h"
#include "models/MA.h"
#include "models/MA2.h"
#include "models/GA.h"

Arr *models_create_new() {
  // Arr[Model]
  Arr *r = arr_new((FPROC)model_free);

  arr_push(r, approx_new());
  arr_push(r, approx3a_new());
  arr_push(r, approx3b_new());
  arr_push(r, approx2_new());
  arr_push(r, minMax_new());
  arr_push(r, mA_new());
  arr_push(r, mA2_new());
  arr_push(r, gA_new());

  return r;
}
