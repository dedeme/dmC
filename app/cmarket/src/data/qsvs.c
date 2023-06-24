// Copyright 05-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/qsv.h"
#include <stddef.h>
#include "kut/arr.h"
#include "data/svs/bmania.h"
#include "data/svs/eleco.h"
#include "data/svs/finan.h"
#include "data/svs/infob.h"
#include "data/svs/inves.h"
#include "data/svs/yahoo.h"

// <Qsv>
Arr *qsvs_list (void) {
  return arr_new_from(
    bmania_new(),
    eleco_new(),
    finan_new(),
    infob_new(),
    inves_new(),
    yahoo_new(),
    NULL
  );
}

Qsv *qsvs_current_diary (void) {
  return infob_new();
}

Qsv *qsvs_current_historic (void) {
  return infob_new();
}
