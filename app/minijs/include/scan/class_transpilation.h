// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Class transpilation process.
/// <p>
/// Documentacion is
/// <a href='http://localhost/mkdocs/minijsDoc/classTranspilation'>here</a>.

#ifndef SCAN_CLASS_TRANSPILATION_H
  # define SCAN_CLASS_TRANSPILATION_H

#include "dmc/ct/Lchar.h"
#include "types/Cpath.h"
#include "types/JsClassResult.h"

/// Makes a class transpilation: Write cache and returns the class.
JsClassResult *class_transpilation_run (Lchar *roots, Cpath *class);

#endif

