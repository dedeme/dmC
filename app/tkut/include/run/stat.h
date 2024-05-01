// Copyright 02-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Statement data. Immutable.

#ifndef RUN_STAT_H
  #define RUN_STAT_H

#include "run/rn_code.h"

///
typedef enum run_stat_type StatT;

///
typedef struct run_stat Stat;

enum run_stat_type {
  ST_RETURN
};

///
struct run_stat {
  int nline;
  int nchar;
  StatT type;
  // <Ex>
  Arr *exprs;
};

/// Constructor.
/// exprs is Arr<Ex>
Stat *stat_new (int nline, int nchar, StatT type, Arr *exprs);

/// Run 'this'
void stat_run(RnCode *code, Stat *this);

///
char *stat_to_js (Stat *this);

///
Stat *stat_from_js (char *js);

#endif
