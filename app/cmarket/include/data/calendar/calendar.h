// Copyright 01-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Market calendar.

#ifndef DATA_CALENDAR_CALENDAR_H
  #define DATA_CALENDAR_CALENDAR_H

#include "kut/arr.h"
#include "data/calendar/timetable.h"

/// Calendar definition.
typedef struct calendar_Calendar {
    Timetable *general; // Default timetable of a market day.
    Arr *holidays; // Arr<char>. Holiday dates.
    Arr *special_days; // Arr<MarketDay>. Days with special timetable.
  } Calendar;

/// Constructor.
///   general     : Default timetable of a market day.
///   holidays    : Arr<char>. Holiday dates.
///   special_days: Arr<MarketDay>. Days with special timetable.
Calendar *calendar_new (Timetable *general, Arr *holidays, Arr *special_days);

/// Constructor from other Calendar modifiyng a field.
///   general     : Default timetable of a market day.
Calendar *calendar_mk_general (Calendar *this, Timetable *general);

/// Constructor from other Calendar modifiyng a field.
///   holidays    : Arr<char>. Holiday dates.
Calendar *calendar_mk_holidays (Calendar *this, Arr *holidays);

/// Constructor from other Calendar modifiyng a field.
///   special_days: Arr<MarketDay>. Days with special timetable.
Calendar *calendar_mk_special_days (Calendar *this, Arr *special_days);

///
char *calendar_to_js (Calendar *this);

///
Calendar *calendar_from_js (char *js);

#endif
