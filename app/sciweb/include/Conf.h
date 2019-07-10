// Copyright 10-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Book configuration.

#ifndef CONF_H
  #define CONF_H

#include "dmc/std.h"

/*--*/

/// Exercise configuration
///   Arguments:
///     page: int
///     number: int
typedef struct Conf_ConfExe ConfExe;

///
ConfExe *confExe_new (int page, int number);

///
int confExe_page (ConfExe *this);

///
int confExe_number (ConfExe *this);

/// Unit configuration
///   Arguments:
///     name: char*
///     exercises: Arr-ConfExe
typedef struct Conf_ConfUnit ConfUnit;

///
ConfUnit *confUnit_new (char *name, Arr *exercises);

///
char *confUnit_name (ConfUnit *this);

/// Arr[ConfExe]
Arr *confUnit_exercises (ConfUnit *this);

/// Book configuration
///   Arguments:
///     sortName: char*
///     longName: char*
///     units: Map-ConfUnit
typedef struct Conf_ConfBook ConfBook;

///
ConfBook *confBook_new (char *sortName, char *longName, Map *units);

///
char *confBook_sortName (ConfBook *this);

///
char *confBook_longName (ConfBook *this);

///
Map *confBook_units (ConfBook *this);

/*--*/

#endif
