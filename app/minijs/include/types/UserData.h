// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Datos taken of user arguments
/// <p>
/// Fields:
///   roots: List of directories with dmjs code.
///   source: Main .dmjs file.
///   target: Target .js file.
#ifndef TYPES_USERDATA_H
  # define TYPES_USERDATA_H

#include "dmc/ct/Lchar.h"
#include "Cpath.h"

/*.-.*/

///
typedef struct userData_UserData UserData;

///
UserData *userData_new(Lchar *roots, Cpath *source, char *target);

///
Lchar *userData_roots(UserData *this);

///
Cpath *userData_source(UserData *this);

///
char *userData_target(UserData *this);

/*.-.*/

#endif

