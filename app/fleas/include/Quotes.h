// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array of Quote

#ifndef QUOTES_H
  # define QUOTES_H

#include "market/Quote.h"

/// Returns an unique array[QUOTES_NUMBER][NICKS_NUMBER].<br>
/// This array is initialized in io_quotes at the begining of program.<br>
/// It is populated with Quotes created with quote_from_str.<br>
/// The quote's and the array are allocated without garbage collector and
/// should be freed with (quotes and array) with quotes_free.
Quote **quotes_get();

#endif


