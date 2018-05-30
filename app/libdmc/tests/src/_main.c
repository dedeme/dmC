// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include "dmc/sys.h"
#include "opt_test.h"
#include "sys_test.h"
#include "str_test.h"
#include "buf_test.h"
#include "arr_test.h"

int main (int argc, char **argv) {

  puts("dmTest. v201805\n");
  sys_init("dmTest");
/*
  opt_test();
  sys_test();
  str_test();
  buf_test();*/
  arr_test();

  return(0);
}
