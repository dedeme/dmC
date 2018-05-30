// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Arr[char]

#ifndef MCHAR_H
  # define MCHAR_H

#include <dmc/all.h>

///
typedef struct mchar_Mchar Mchar;

///
Mchar *mchar_new(void);

///
size_t mchar_size(Mchar *this);

///
bool  mchar_contains(Mchar *this, char *key);

///
void mchar_put(Mchar *this, char *key, char *value);

/// mchar_get returns the value of 'key' or the same 'key' if such key does not
/// exist.
char *mchar_get(Mchar *this, char *key);

///
Arr/*Json*/ *mchar_serialize(Mchar *this);

///
Mchar *mchar_restore(Arr/*Json*/ *j);

#endif

