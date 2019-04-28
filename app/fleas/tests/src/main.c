// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "main.h"
#include "gen_tests.h"
#include "qquotes_tests.h"

int main(int argc, char **args) {
  sys_init("fleas2_tests");

//  gen_tests();
  qquotes_tests();

  sys_end();
  return 1;
}
