// Copyright 01-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Calendar timetable.

#ifndef DATA_CALENDAR_TIMETABLE_H
  #define DATA_CALENDAR_TIMETABLE_H

/// Timetable definition.
typedef struct timetable_Timetable {
    int hopen; // Open hour.
    int mopen; // Open minute.
    int hclose; // Close hour.
    int mclose; // Close minue.
  } Timetable;

/// Constructor.
///   hopen : Open hour.
///   mopen : Open minute.
///   hclose: Close hour.
///   mclose: Close minute.
Timetable *timetable_new (
    int hopen, int mopen, int hclose, int mclose
  );

///
char *timetable_to_js (Timetable *this);

///
Timetable *timetable_from_js (char *js);

#endif
