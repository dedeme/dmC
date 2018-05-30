// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Arr[Att]

#ifndef AST_AATT_H
  # define AST_AATT_H

#include "Att.h"

///
typedef struct aatt_Aatt Aatt;

///
Aatt *aatt_new(void);

///
size_t aatt_size(Aatt *this);

///
void aatt_add(Aatt *this, Att *a);

///
Att *aatt_get(Aatt *this, int ix);

///
Att *aatt__get(Aatt *this, char *id);

///
void aatt_set(Aatt *this, int ix, Att *a);

///
Arr/*Json*/ *aatt_serialize(Aatt *this);

///
Aatt *aatt_restore(Arr/*Json*/ *j);


#endif

