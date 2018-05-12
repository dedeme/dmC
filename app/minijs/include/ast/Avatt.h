// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Arr[Vatt]

#ifndef AST_AVATT_H
  # define AST_AVATT_H

#include "Vatt.h"

///
typedef Arr Avatt;

///
Avatt *avatt_new(void);

///
void avatt_add(Avatt *this, Vatt *t);

///
Vatt *avatt_get(Avatt *this, int ix);

///
Arr/*Json*/ *avatt_serialize(Avatt *this);

///
Avatt *avatt_restore(Arr/*Json*/ *j);


#endif

