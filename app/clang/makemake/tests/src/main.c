// Copyright 07-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "main.h"
#include "node1_tests.h"
#include "node2_tests.h"
#include "reader_dependency_tests.h"

int main () {
  exc_init();

  node1_tests();
  node2_tests();
  reader_dependency_tests();
}
