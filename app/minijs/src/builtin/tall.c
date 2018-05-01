// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "builtin/tall.h"
#include "builtin/tbool.h"
#include "builtin/tbyte.h"
#include "builtin/tint.h"
#include "builtin/tfloat.h"

void tall_add(Arr/*class*/ *builtins) {
  tbool_add(builtins);
  tbyte_add(builtins);
  tint_add(builtins);
  tfloat_add(builtins);
}
