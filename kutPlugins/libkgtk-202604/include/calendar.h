// Copyright 10-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Calendar widget.

#include "exp.h"

#ifndef CALENDAR_H
  #define CALENDAR_H

/// Creates a new calendar.
/// \ -> <gwg>
Exp *libkgtk_calendar_new (Arr *params);

/// Sets 'w' date.
///   w   : Widged to set.
///   date: String in format YYYYMMDD.
/// \<wg>, s -> <wg>
Exp *libkgtk_calendar_date (Arr *params);

/// Returns Exp<s> with date in format YYYYMMDD.
///   w : Calendadr.
/// \<wg> -> s
Exp *libkgtk_calendar_get_date (Arr *params);

#endif
