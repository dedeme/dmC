// Copyright 28-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// IO operations.

#ifndef IO_H
  #define IO_H

#include "dmc/std.h"
#include "market/Quote.h"
#include "Fbest.h"

///
void io_init(void);

///
void io_end(void);

/// Every column of the returned array is a nick and there are io_qnicks
/// number of them.<p>
/// Every row of the returned array is a date and there are io_qdays number
/// of them.
Quote **io_quotes(void);

/// Days number of quotes.
int io_qdays(void);

/// Nicks number of quotes.
int io_qnicks(void);

///
void io_write_fleas(
  char *days,
  int cycle,
  // Arr[Flea]
  Arr *fleas
);

///
void io_read_fleas(
  int *cycle,
  // Arr[Flea]
  Arr **fleas_new,
  char *days
);

/// 'results' is Arr[Fresult]
void io_write_results(char *group, Arr *results);

/// Reads data from start bprocess.
///   bests: Previous bests or [] of date is ""
///   date: Date to calculate or "" if there are no new date to calculate.
///   results: Results of process or [] if date is ""
///   days: Group to process
void io_best_start(
  // Arr[Fbest]
  Arr **bests,
  char **date,
  // Arr[Fresults]
  Arr **results,
  int days
);

void io_write_best(int days, Fbest *best);

/// Returns 'true' if application was locked, otherwise blocks it.
int io_lock(void);

/// Unlocks application.
void io_unlock(void);

/// Sends message to stop the application.
void io_send_stop(void);

/// Returns 'true' if 'io_send_stop' was called
int io_stop(void);

/// Removes stop mark
void io_del_stop(void);

/// Remove all files
void io_clean(void);

#endif
