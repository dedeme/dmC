// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Class entry reading process.
/// <p>
/// Documentacion is
/// <a href='http://localhost/mkdocs/minijsDoc/rentry'>here</a>.

#ifndef SCAN_ENTRY_READING_H
  # define SCAN_ENTRY_READING_H

#include "types/UserData.h"
#include "types/Errors.h"
#include "types/Dep.h"
#include "types/JsEntryResult.h"

/// Makes transpilation.
JsEntryResult *entry_reading_run (
  UserData *userData, Errors *errors, Dep *function
);

#endif
