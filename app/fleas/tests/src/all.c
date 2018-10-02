// Copyright 25-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "all.h"
#include "dmc/std.h"

#include "testIo.h"
#include "testGen.h"
#include "testFlea.h"
#include "testUpDown.h"

int main (int argc, char **argv) {
  sys_init("fleasTest");

  testIo_run();
  testGen_run();
  testFlea_run();
  testUpDown_run();
}
