// Copyright 02-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "sub/mul.h"
#include "kut/arr.h"

Val libplug_mul (Val vs) {
  Val *a = (vs.a)->begin;
  Val n1 = *a++;
  Val n2 = *a;
  return (Val)(n1.i * n2.i);
}
