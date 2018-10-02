// Copyright 26-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Input-Output management

#ifndef IO_H
  #define IO_H

#include "dmc/std.h"
#include "data/Aaquote.h"
#include "data/Aflea.h"

///
typedef struct io_Group Group;

///
void io_init(void);

///
void io_clean(void);

/// Control to avoid double runs. Returns 'true' if there is not duplicity.
bool io_lock(void);

/// Remove the lock used to control double runs.
void io_unlock(void);

/// Send a signal to stop the program
void io_send_stop(void);

/// Remove the stop signal.
void io_remove_stop(void);

/// Return true if the program has been stoped
bool io_stoped(void);

/// Returns a list of groups of companies.
///   groups: Array with all the families
///   groups_number: The families number
void io_groups_list(Group ***groups, int *groups_number);

///
char *io_group_to_str(Group *group);

/// NOTE: DON' USE!!!. This functions has been made public, for testing raisons.
/// Use instead io_read_quotes.
/// Reads from quotes database nicks and nick model.
///   model: Index of nick model in 'nicks'
///   nicks: List with all the nicks which match 'selection'
///   data_dir: Data directory of program 'Quotes'
///   sel: Niks type to select
/// Usage:
///   int model;
///   Achar *nicks;
///   io_read_nicks(&model, &nicks, "some/path", IO_ALL);
void io_read_nicks(
  int *model,
  Achar **nicks,
  char *data_dir,
  Group *sel
);

/// Reads from quotes database nicks and quotes values.
///   nicks: List with all the nicks which match 'selection'
///   dates: List with all the dates which match 'selection' ordered from
///     before to after.
///   quotes: Two dimensional array whose rows are days (with the same number
///     and the same order as 'dates') and whose cols are nicks (with in the
///     same number and the same order as 'nicks').
///   data_dir: Data directory of program 'Quotes'
///   sel: Niks type to select
/// Usage:
///   Achar *nicks;
///   Achar *dates;
///   Aaqval *qvals;
///   io_read_nicks(&nicks, &dates, &qvals, "some/path", IO_ALL);
void io_read_quotes(
  Achar **nicks,
  Achar **dates,
  Quote ****quotes,
  char *data_dir,
  Group *sel
);

/// Writes family data
void io_write_family(Group *gr, Family *fm, int cycle, Aflea *fleas);

/// Read family data
void io_read_family(int *cycle, Aflea **fleas, Group *gr, Family *fm);

///
void io_write_historic(Group *gr, int cycle, Arr/*HistoricEntry*/ *historic);

/// Key is the number of cycle.
Map/*Arr[HistoricEntry]*/ *io_read_historic(Group *gr);

#endif
