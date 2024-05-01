// Copyright 27-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Quotes reader.


#ifndef QUOTES_READER_H
  #define QUOTES_READER_H

#include "kut/map.h"

/// Returns the quotes requested normalized (removing -1).
/// Input:
///   - path (s)   : Directory of quotes.
///   - nquotes (i): 2 -> read open and closes. 4 -> read open, close, maxs and mins.
///   - Cos ([s.]) : Companies to read quotes.
/// Output:
///   - Dates ([s.])    : Dates of quotes.
///   - Cos ([s.])      : Companies read.
///   - Opens ([[n.].]) : Opens matrix of Dates(rows)xCos(columns).
///   - Closes ([[n.].]): Closes matrix of Dates(rows)xCos(columns).
///   - Maxs ([[n.].])  : Maxima matrix of Dates(rows)xCos(columns). (For nquotes == 4)
///   - Mins ([[n.].])  : Minima matrix of Dates(rows)xCos(columns). (For nquotes == 4)
///   - Errors ([s.])   : Detected errors.
/// 'Cos' requested and returnd can not match. In such case one o more error
/// messages are registered in 'Errors'.
char *quotesReader_run(Map *rq);

#endif
