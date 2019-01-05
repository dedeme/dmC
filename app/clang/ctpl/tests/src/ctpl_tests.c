// Copyright 21-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ctpl_tests.h"

#include "stName_tests.h"
#include "arg_tests.h"
#include "var_tests.h"
#include "fun_tests.h"

int main(int argc, char *argv[]) {
  stName_tests();
  arg_tests();
  var_tests();
  fun_tests();
}
