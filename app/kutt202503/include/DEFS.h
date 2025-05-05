// Copyright 10-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Definitions.

#ifndef DEFS_H
  #define DEFS_H

  #define GC_THREADS
  #include "kut/DEFS.h"
  ///
  #define exc_kutt_t "kutt"
  ///
  #define EXC_KUTT(msg) \
    THROW(exc_kutt_t, msg)

#endif
