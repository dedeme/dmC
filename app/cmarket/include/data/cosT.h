// Copyright 05-06-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Companies table data.

#ifndef DATA_COST_H
  #define DATA_COST_H

#include "kut/arr.h"

///
typedef struct cosT_CosT {
    // Nick of selected company to verify dates.
    char *main;
    // <Co>. Companies.
    Arr *cos;
  } CosT;

/// Constructor.
///   main: Nick of selected company to verify dates.
///   cos: Arr<Co>. Companies.
CosT *cosT_new (char *main, Arr *cos);

///
char *cosT_to_js (CosT *this);

///
CosT *cosT_from_js (char *js);

#endif
