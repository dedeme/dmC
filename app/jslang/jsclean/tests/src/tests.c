// Copyright 06-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests.h"

#include "params_tests.h"
#include "io_tests.h"

int main (int argc, char *argv[]) {
  exc_init();

  params_tests();
  io_tests();
}
