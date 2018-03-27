// Copyright 21-Feb-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <tests_all.h>
#include "tests_types.h"
#include "tests_value.h"
#include "tests_strings.h"
#include "tests_numbers.h"
#include "Cpath.h"

int main() {
  sys_init("minijs tests");

  Arr/*char*/ *paths = arr_new();
  arr_add(paths, "../test1");
  cpath_init(paths);
/*
  tests_types();
  tests_value();
  tests_strings();*/
  tests_numbers();
  return 0;
}
