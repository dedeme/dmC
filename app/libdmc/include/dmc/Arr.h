// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array structure

#ifndef DM_ARR_H
  # define DM_ARR_H

#include <stdbool.h>
#include <stddef.h>

typedef struct it_It It;

///
typedef struct arr_Arr Arr;

/// arr_new initializes an array
Arr *arr_new (void);

/// arr_new initializes an array with an intial buffer.
///   this   : object created
///   buffer : Initial buffer size. Its default is 15.
Arr *arr_new_buf (size_t size_buf);

/// arr_size returns 'this' size
size_t arr_size (Arr *this);

/// arr_add adds an element. 'element' can not be NULL.<p>
/// Throws exception.
void arr_add (Arr *this, void *element);

/// arr_add_arr dds an array
void arr_add_arr (Arr *this, Arr *another);

/// arr_get returns element at position 'index'. Tests limits.<p>
void *arr_get (Arr *this, size_t index);

/// arr_set replaces element at 'index' by a new 'element'. Tests limits
/// and 'element'
/// can not be NULL.<p>
void arr_set (Arr *this, size_t index, void *element);

/// arr_insert inserts an element at 'index'. Tests limits and 'element' can
/// no be NULL.<p>
void arr_insert (Arr *this, size_t index, void *element);

/// arr_insert_arr inserts an array at 'index'. Tests limits.<p>
void arr_insert_arr (Arr *this, size_t index, Arr *another);

/// arr_remove removes the elemente at 'index'. Tests limits.<p>
void arr_remove (Arr *this, size_t index);

/// arr_remove_range removes elements between 'begin' (inclusive) and 'end'
/// (exclusive). Tests limits.<p>
void arr_remove_range (Arr *this, size_t begin, size_t end);

/// arr_reverse reverses elements of 'this'
void arr_reverse (Arr *this);

/// arr_sort sorts elements of 'this' according 'f'
///   f: Function which returns 'true' if the order is ascendent and e1 > e2,
///      or the order is descendent and e2 > e1
void arr_sort (Arr *this, bool (*f)(void *e1, void *e2));

/// arr_shuflle remix 'this' elements. It should be used after calling
/// <tt>rnd_init()</tt> or <tt>sys_init()</tt>
void arr_shuffle (Arr *this);

///
It *arr_to_it (Arr *this);

///
Arr *arr_from_it (It *it);

#endif
