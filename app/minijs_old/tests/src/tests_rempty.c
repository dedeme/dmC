// Copyright 2-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <assert.h>
#include "tests_rempty.h"
#include "io/main.h"

void tests_rempty() {
  puts("Reader: empty file");

  char *pars[] = {"", "-p", "data", "Empty"};
  main_start(4, pars);

  puts("    Finished");
}
