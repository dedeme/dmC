// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Arr[Att]

#ifndef AST_AATT_H
  # define AST_AATT_H

#include "Att.h"

///
typedef Arr Aatt;

///
Aatt *aatt_new(void);

///
void aatt_add(Aatt *this, Att *t);

///
Att *aatt_get(Aatt *this, int ix);

///
Att *aatt__get(Aatt *this, char *id);

///
Json *aatt_serialize(Aatt *this);

///
Aatt *aatt_restore(Json *j);


#endif

