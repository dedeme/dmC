// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Token reader.

#ifndef TKREADER_H
  #define TKREADER_H

#include "dmc/async.h"
#include "Reader.h"

/// Returns Opt<Token> - Reads one token. If there are no
/// more tokens, returns 'opt_empty()'.
Opt *tkreader_next(Reader *reader);

#endif
