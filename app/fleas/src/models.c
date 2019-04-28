// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "models.h"
#include "Model.h"
#include "models/AppMM.h"
#include "models/AppMM1.h"
#include "models/AppMM1B.h"
#include "models/AppMM2B.h"
#include "models/AppMMB.h"
#include "models/AppMMB2.h"
#include "models/AppMMS.h"
#include "models/AppMMS1.h"
#include "models/Approx.h"
#include "models/Approx3a.h"
#include "models/Approx3b.h"
#include "models/Approx2.h"
#include "models/MinMax.h"
#include "models/MinMax2.h"
#include "models/MA.h"
#include "models/MA2.h"
#include "models/GA.h"
#include "models/SupRes3.h"

Arr *models_create_new() {
  // Arr[Model]
  Arr *r = arr_new((FPROC)model_free);

  arr_push(r, appMM_new());
  arr_push(r, appMM1_new());
  arr_push(r, appMM1B_new());
  arr_push(r, appMM2B_new());
  arr_push(r, appMMB_new());
  arr_push(r, appMMB2_new());
  arr_push(r, appMMS_new());
  arr_push(r, appMMS1_new());
  arr_push(r, approx_new());
  arr_push(r, approx2_new());
  arr_push(r, approx3a_new());
  arr_push(r, approx3b_new());
//  arr_push(r, gA_new());
//  arr_push(r, mA_new());
  arr_push(r, mA2_new());
  arr_push(r, minMax_new());
//  arr_push(r, minMax2_new());
//  arr_push(r, supRes3_new());

  return r;
}
