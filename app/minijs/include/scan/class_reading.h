// Copyright 27-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Class reading process.
/// <p>
/// Documentacion is
/// <a href='http://localhost/mkdocs/minijsDoc/classReading'>here</a>.

#ifndef SCAN_CLASS_READING_H
  # define SCAN_CLASS_READING_H

#include "types/Skeleton.h"
#include "types/Scanner.h"
#include "types/JsClassResult.h"

/// Makes a class reading.
JsClassResult *class_reading_run (
  Skeleton *skeleton, Scanner *scanner
);

#endif

