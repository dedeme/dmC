// Copyright 05-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Quote servers data.

#ifndef DATA_QSVS_H
  #define DATA_QSVS_H

#include "kut/arr.h"
#include "data/qsv.h"

/// Returns Arr<Qsv> of quotes servers.
Arr *qsvs_list (void);

/// Returns the current active diary server
Qsv *qsvs_current_diary (void);

/// Returns the current active historic server
Qsv *qsvs_current_historic (void);

#endif
