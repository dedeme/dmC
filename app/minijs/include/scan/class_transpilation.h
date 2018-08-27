// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Class transpilation process.
/// <p>
/// Documentacion is
/// <a href='http://localhost/mkdocs/minijsDoc/classTranspilation'>here</a>.

#ifndef SCAN_CLASS_TRANSPILATION_H
  # define SCAN_CLASS_TRANSPILATION_H

#include "types/UserData.h"
#include "types/Errors.h"
#include "types/Cpath.h"
#include "types/JsClassResult.h"

/// Makes a class transpilation: Write cache and returns the class.
JsClassResult *class_transpilation_run (
  UserData *userData, Errors *errors, Cpath *class
);

#endif

