// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "models.h"
#include "Model.h"
#include "models/MM2A.h"
#include "models/MM2Aa.h"
#include "models/MM2Ab.h"
#include "models/MM1.h"
#include "models/MMBack1.h"
#include "models/MMBack2.h"
#include "models/MMBack2a.h"
#include "models/MMBack2b.h"
#include "models/MM4.h"
#include "models/MM2B.h"
#include "models/MMWin2.h"
#include "models/MMWin1.h"
#include "models/MMWin2a.h"
#include "models/MMWin2b.h"
#include "models/Approx.h"
#include "models/Approx3a.h"
#include "models/Approx3b.h"
#include "models/Approx2.h"
#include "models/Incr3.h"
#include "models/Incr2.h"
#include "models/MA.h"
#include "models/MA2.h"
#include "models/GA.h"
#include "models/SupRes3.h"

Arr *models_create_new() {
  // Arr[Model]
  Arr *r = arr_new((FPROC)model_free);

  arr_push(r, mM2A_new());
  arr_push(r, mM2Aa_new());
  arr_push(r, mM2Ab_new());
  arr_push(r, mM1_new());
  arr_push(r, mMBack1_new());
  arr_push(r, mMBack2_new());
  arr_push(r, mMBack2a_new());
  arr_push(r, mMBack2b_new());
  arr_push(r, mM4_new());
  arr_push(r, mM2B_new());
  arr_push(r, mMWin2_new());
  arr_push(r, mMWin1_new());
  arr_push(r, mMWin2a_new());
  arr_push(r, mMWin2b_new());
  arr_push(r, approx_new());
  arr_push(r, approx2_new());
  arr_push(r, approx3a_new());
  arr_push(r, approx3b_new());
//  arr_push(r, gA_new());
//  arr_push(r, mA_new());
  arr_push(r, mA2_new());
  arr_push(r, incr3_new());
//  arr_push(r, incr2_new());
//  arr_push(r, supRes3_new());

  return r;
}
