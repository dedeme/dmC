// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdlib.h>
#include <time.h>
#include "dmc/rnd.h"

void rnd_init () {
  srand(time(0));
}

inline
double rnd_d () {
  return (double)rand() / (double)RAND_MAX;
}

inline
size_t rnd_i (size_t top) {
  return (size_t)(rnd_d() * top);
}
