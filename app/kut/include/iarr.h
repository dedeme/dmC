// Copyright 12-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Immutable array of 'int'.

#ifndef IARR_H
  #define IARR_H

typedef struct iarr_Iarr Iarr;

/// Constructor of a Iarr with n (> 0) elements..
Iarr *iarr_new (int n, int *ints);

/// Constructor of a Iarr with n (> 0) elements.
/// Ejemplo: iarr_new_from(3, 100, 45, -15);
Iarr *iarr_new_from (int n, ...);

/// Returns the size of 'this'.
int iarr_size (Iarr *this);

/// Returns the value at index 'ix'
int iarr_get (Iarr *this, int ix);

#endif
