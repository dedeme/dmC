// Copyright 13-Jan-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef SERVERS_READER_H
  #define SERVERS_READER_H

#include "dmc/std.h"
#include "dmc/Darr.h"

/// Returns 1 if reading is wrong, otherwise returns 0 <p>
/// 'codes_new' and 'qs_new' are initialized in any case.
int reader_run(
  // Arr[char]
  Arr **codes_new,
  Darr **qs_new,
  char *path,
  int isIso,
  // Varr[char]
  Varr *tstart,
  // Varr[char]
  Varr *tend,
  // Varr[char]
  Varr *rstart,
  // Varr[char]
  Varr *rend,
  // Varr[char]
  Varr *c1start,
  // Varr[char]
  Varr *c1end,
  // Varr[char]
  Varr *c2start,
  // Varr[char]
  Varr *c2end
);

#endif
