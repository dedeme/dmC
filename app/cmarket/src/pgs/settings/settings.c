// Copyright 01-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "pgs/settings/settings.h"
#include "pgs/settings/calendarPg/calendarPg.h"
#include "pgs/settings/nicks/nicks.h"
#include "pgs/settings/nicks/nkeditor.h"
#include "pgs/settings/serversPg.h"
#include "pgs.h"
#include "db/log.h"

// mrq is Map<char>
void settings_process (Map *mrq) {
  char *source = pgs_rs(mrq, "source");
  if (str_eq(source, "CalendarPg")) calendarPg_process(mrq);
  else if (str_eq(source, "Nicks")) nicks_process(mrq);
  else if (str_eq(source, "Nicks/Editor")) nkeditor_process(mrq);
  else if (str_eq(source, "Servers")) serversPg_process(mrq);
  else
    EXC_GENERIC(str_f(
      "Value '%s' not found for 'source' in '%s'", source, js_wo(mrq)
    ));
}
