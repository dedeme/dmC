// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Class skeleton reading process.
/// <p>
/// Documentacion is
/// <a href='http://localhost/mkdocs/minijsDoc/skeletonReading'>here</a>.

#ifndef SCAN_SKELETON_READING_H
  # define SCAN_SKELETON_READING_H

#include "dmc/ct/Lchar.h"
#include "types/Scanner.h"
#include "types/SkeletonResult.h"

/// Makes a skeleton reading.
SkeletonResult *skeleton_reading_run (Lchar *roots, Scanner *scan);

#endif

