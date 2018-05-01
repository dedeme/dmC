// Copyright 30-Apr-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_all.h"
#include "tests_imported.h"
#include "tests_program.h"
#include "tests_type.h"
#include "tests_builtins.h"

int main() {
  sys_init("minijs tests");
/*  tests_imported();
  tests_program();
  tests_type();*/
  tests_builtins();
  return 0;
}
