// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "main.h"
#include "dmCryp_tests.h"

int main(int argc, char **args) {
  sys_init("dmCryp_tests");

  dmCryp_tests();

  sys_end();
  return 1;
}
