// Copyright 11-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests/all.h"
#include "tests/ttoken.h"
#include "tests/ttypes.h"
#include "tests/texps.h"
#include "tests/tstats.h"
#include "tests/togrouping.h"

void all_run() {
  ttoken_run();
  ttypes_run();
  texps_run();
  tstats_run();
  togrouping_run();
}
