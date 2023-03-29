// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef CLOCK_H
  #define CLOCK_H

/// Start counting barbery open time.
void clock_run(void);

/// Returns TRUE if the time for barbery is over.
int clock_is_time_over (void);

#endif
