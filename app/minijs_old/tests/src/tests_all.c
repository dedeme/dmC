// Copyright 03-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_all.h"
#include "dmc/sys.h"
#include "tests_rempty.h"

int main() {
  sys_init("minijs_tests");

  tests_rempty();

  return 0;
}
