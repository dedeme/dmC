// Copyright 29-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests.h"
#include "main.h"
#include "primitives.h"
#include "reader_tests.h"

int main () {
  exc_init();
  primitives_init();

  reader_tests();

  return 0;
}
