// Copyright 01-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Calendar market day.

#ifndef DATA_CALENDAR_MARKETDAY_H
  #define DATA_CALENDAR_MARKETDAY_H

/// MarketDay definition.
typedef struct marketDay_MarketDay {
    char *date; // Day.
    int hopen; // Open hour.
    int mopen; // Open minute.
    int hclose; // Close hour.
    int mclose; // Close minue.
  } MarketDay;

/// Constructor.
///   date  : Day.
///   hopen : Open hour.
///   mopen : Open minute.
///   hclose: Close hour.
///   mclose: Close minute.
MarketDay *marketDay_new (
    char *date, int hopen, int mopen, int hclose, int mclose
  );

///
char *marketDay_to_js (MarketDay *this);

///
MarketDay *marketDay_from_js (char *js);

#endif
