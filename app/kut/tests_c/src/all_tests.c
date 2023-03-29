// Copyright 27-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "all_tests.h"
#include <stdio.h>
#include "kut/DEFS.h"
#include "kut/sys.h"
#include "fileix.h"
#include "modules.h"
#include "fileix_tests.h"
#include "exp_tests.h"
#include "heap0_tests.h"
#include "heap_tests.h"
#include "stat_tests.h"
#include "token_tests.h"
#include "module_tests.h"
#include "cdr_tests.h"

int main(int argc, char *argv[]) {
  sys_init();
  fileix_init();
  modules_init();

  fileix_tests();
  exp_tests();
  heap0_tests();
  heap_tests();
  stat_tests();
  token_tests();
  module_tests();
  cdr_tests();
}

