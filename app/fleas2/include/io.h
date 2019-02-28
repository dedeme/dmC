// Copyright 24-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef IO_H
  #define IO_H

#include "dmc/std.h"

/// Initializes files
///   fmodels: Arr[Model] Flea models.
void io_init(Arr *fmodels);

/// Free resources
void io_free(void);

/// Returns the nick number
int io_qnicks(void);

/// Returns the days number
int io_qdays(void);

/// Returns a opens array
double *io_opens(void);

/// Returns a closes array
double *io_closes(void);

/// Returns an Arr[char]. Its indexes match with columns of opens and closes.
Arr *io_nicks(void);

/// Returns an Arr[char]. Its indexes match with rows of opens and closes.
Arr *io_dates(void);

/// Reads bests results. Returns an Arr *rss
///   model: It can be a 'model_name' or "" (for bests)
Arr *io_rrss_new(char *model);

/// Writes bests results.
///   is_best: Its value is 1 if rs is a bests list.
///   model: It can be a 'model_name' or "" (for bests)
///   rs: Arr[BestsRs]
void io_wrss(int is_bests, char *model, Arr *rss);

/// Writes data for charts
void io_wcharts(char *model, char *data);

#endif
