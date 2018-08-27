// Copyright 03-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "minijs.h"
#include "io/main.h"
#include "dmc/sys.h"

int main (int argc, char **argv) {
  sys_init("minijs");
  return main_start(argc, argv);
}
