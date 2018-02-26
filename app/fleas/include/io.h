// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Input - output operations

#ifndef IO_H
  # define IO_H

#include "Nick.h"
#include "Flea.h"

/// io_init intializes and checks program files.
void io_init(void);

///
Nicks *io_nicks(void);

/// io_quotes intializes the object returned by quotes_get() and returns
/// an array with the list of dates of such quotes.
Arr/*char*/ *io_quotes(Nicks *nicks);

///
void io_get_fleas(size_t *fleaId, size_t *cycle, Fleas **fleas);

///
void io_set_fleas(size_t fleaId, size_t cycle, Fleas *fleas);

///
void io_save_bests(size_t cycle, Json *data);

///
void io_save_traces(Json *data);

///
void io_backup(char *dir);

/// 'file' must include '.zip' extension.
void io_restore(char *file);

/// Removes duplicate fleas
void io_duplicates(void);

/*
/// io_start writes a lock file to control double starting of the program.
void io_start(void);

*/
#endif
