// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Arr[char]

#ifndef ACHAR_H
  # define ACHAR_H

#include <dmc/all.h>

///
typedef Arr Achar;

///
Achar *achar_new(void);

///
bool achar_contains(Achar *this, char *s);

///
void achar_add(Achar *this, char *s);

///
char *achar_get(Achar *this, int ix);

///
Json *achar_serialize(Achar *this);

///
Achar *achar_restore(Json *j);


#endif

