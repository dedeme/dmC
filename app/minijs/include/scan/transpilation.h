// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Transpilation process.
/// <p>
/// Documentacion is
/// <a href='http://localhost/mkdocs/minijsDoc/transpilation'>here</a>.

#ifndef SCAN_TRANSPILATION_H
  # define SCAN_TRANSPILATION_H

#include "types/UserData.h"
#include "types/Fails.h"

/// Makes transpilation.
Fails *transpilation_run (UserData *userData);

#endif

