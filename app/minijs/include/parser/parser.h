// Copyright 13-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Parser

#ifndef PARSER_PARSER_H
  # define PARSER_PARSER_H

#include <dmc/all.h>
#include "Cpath.h"
#include "Obj.h"

/// parser_dtype
///   local: It is true if path can be called although it is local
///   path: Id of class
///   object: Property or method of path
/// This function fail with an excetion if Type can not be known.
Obj *parser_type(bool local, Cpath *path, char *object);

#endif
