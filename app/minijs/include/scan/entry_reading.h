// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Class entry reading process.
/// <p>
/// Documentacion is
/// <a href='http://localhost/mkdocs/minijsDoc/rentry'>here</a>.

#ifndef SCAN_ENTRY_READING_H
  # define SCAN_ENTRY_READING_H

#include "dmc/ct/Lchar.h"
#include "types/Dep.h"
#include "types/JsEntryResult.h"

/// Read a class entry.
JsEntryResult *entry_reading_run (Lchar *roots, Dep *function);

#endif
