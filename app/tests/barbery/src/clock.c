// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "clock.h"
#include "kut/DEFS.h"
#include "DEFS.h"
#include "kut/sys.h"

static int time_over = FALSE;

void clock_run(void) {
  sys_sleep(CLOCK_TIME);
  puts("Clock: Time is over");
  time_over = TRUE;
}

int clock_is_time_over (void) {
  return time_over;
}
