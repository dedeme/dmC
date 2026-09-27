// Copyright 05-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Quotes reader.

#ifndef QUOTES_H
  #define QUOTES_H

#include "exp.h"

/// Converter.
/// Returns Exp<[[s.], [s.], [[f.].], [[f.].], [[f.].], [[f.].]]> ::
///         Exp<[cos, dates, opens, closes, maxs, mins]>
///   params: Arr<<cquotes>>.
Exp *libmkt_quotes_from_cquotes(Arr *params);

/// Reads quotes.
/// Returns Exp<[[s.], [s.], [[f.].], [[f.].], [[f.].], [[f.].]]> ::
///         Exp<[cos, dates, opens, closes, maxs, mins]>
///   params: Arr<s, [s.]>. Fields are:
///             dpath : Directory with files 'NICK'.tb.
///             cos   : Nicks of companies to read.
Exp *libmkt_quotes_read (Arr *params);

#endif
