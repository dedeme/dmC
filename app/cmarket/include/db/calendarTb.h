// Copyright 01-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Calendar table.

#ifndef DB_CALENDARTB_H
  #define DB_CALENDARTB_H

#include "data/calendar/calendar.h"

///
void calendarTb_init (void);

///
char *calendarTb_read_js (void);

///
Calendar *calendarTb_read (void);

///
void calendarTb_write_js (char *js);

///
void calendarTb_write (Calendar *data);

#endif
