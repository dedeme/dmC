// Copyright 26-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "fleas_tests.h"

#include "gen_tests.h"
#include "co_tests.h"
#include "buySorter_tests.h"
#include "flea_tests.h"
#include "sorter_tests.h"

///
int main(int argc, char *arg[]) {
  sys_init("fleas_tests");
/*
  gen_tests();
  co_tests();
  buySorter_tests();
  flea_tests();*/
  sorter_tests();

  sys_end();
}
