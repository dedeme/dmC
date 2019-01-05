// Copyright 21-Nov-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Fleas results sorter

#ifndef SORTER_H
  #define SORTER_H

#include "dmc/std.h"

///
typedef struct sorter_Sorter Sorter;

/// Creates a new Sorter
///   size: Maximum number of elements
///   ffree: Function to free elements of sorter
Sorter *sorter_new(int size, void (*ffree)(void *));

/// Adds an 'element' to 'this'. If element is added, it is returned when
/// 'sorter_free_new' is called. Otherwise 'element' is immeditely freed.
void sorter_add(Sorter *this, void *element, double ponderation);

/// Returns an Arr[void] sorted from lowest to greatest and free resources.<p>
/// The array returned must be freed with arr_free.
Arr *sorter_free_new(Sorter *this);

#endif
