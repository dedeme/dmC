// Copyright 10-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ct/Astat.h"
#include "ast/Stat.h"

#define TY Stat
#define FN stat
#include "dmc/tpl/tarr.c"
#undef TY
#undef FN

Ajson *astat2_to_json(Astat *this) {
  return astat_to_json(this, stat_to_json);
}

Astat *astat2_from_json(Ajson *js) {
  return astat_from_json(js, stat_from_json);
}
