// Copyright 06-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef DATA_PATH_H
  #define DATA_PATH_H

#include "dmc/std.h"

/*--*/

///
///   Arguments:
///     absolute: char*
///     relative: char*
typedef struct Path_Path Path;

///
Path *path_new (char *absolute, char *relative);

///
char *path_absolute (Path *this);

///
char *path_relative (Path *this);

/*--*/

/// Arr[char] Returns duplicates paths in 'paths'
Arr *path_duplicates (Arr *paths);

#endif
