// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Paths management
/// <p>
/// Fields:
///   root: Parent directory.
///   relative: Path relative to root.
#ifndef TYPES_CPATH_H
  # define TYPES_CPATH_H

#include "stdbool.h"

/*.-.*/

#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"

///
typedef struct cpath_Cpath Cpath;

///
Cpath *cpath_new(char *root, char *relative);

///
char *cpath_root(Cpath *this);

///
char *cpath_relative(Cpath *this);

///
Ajson *cpath_to_json(Cpath *this);

///
Cpath *cpath_from_json(Ajson *s);

/*.-.*/

/// Returns the complete path of this (root/relative).
char *cpath_complete(Cpath *this);

/// Equality
bool cpath_eq(Cpath *this, Cpath *other);

#endif

