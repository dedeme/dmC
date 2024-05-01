// Copyright 27-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Volumes reader.


#ifndef VOLUMES_H
  #define VOLUMES_H

#include "kut/map.h"

/// Returns the averages of volume for each company in function of indicated
/// samples.
/// Input:
///   - path (s)   : Directory of quotes.
///   - samples (i): Number of samples to read.
///   - Cos ([s.]) : Companies to read volumes.
/// Output:
///   - Vols ([f.]): Average values for each company in 'Cos'. If a company can
///                  be read, its volume is 0.
char *volumes_read(Map *rq);

#endif
