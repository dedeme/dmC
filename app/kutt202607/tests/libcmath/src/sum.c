// Copyright 02-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "sum.h"
#include <stdio.h>

Val libplug_const (Val vs) {
  return (Val)"Kutt plugin ok.";
}

Val libplug_echo (Val vs) {
  puts((*(vs.a)->begin).s);
  return (Val)0;
}

Val libplug_sum (Val vs) {
  Val *a = (vs.a)->begin;
  Val n1 = *a++;
  Val n2 = *a;
  return (Val)(n1.i + n2.i);
}
