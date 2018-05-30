// Copyright 30-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Iterator.<p>
/// Iterator does not admit NULL values.

#ifndef DM_IT_H
  #define DM_IT_H

#include <stddef.h>
#include <stdbool.h>

typedef struct opt_Opt Opt;

///
typedef struct it_It It;

/// it_new creates a new It.
///   o: Container
///   next: function which returns a element of 'o' or NULL if there is no
///         more element.
It *it_new(void *o, void *(*next)(void *o));

///
It *it_empty(void);

///
It *it_unary(void *e);

/// it_range is an iterator that returns values between begin (inclusive)
/// and end (exclusive)
It *it_range(int begin, int end);

/// it_range0 is equals to it_range(0, end);
It *it_range0(int end);

///
bool it_has_next (It *this);

///
void *it_next (It *this);

// Show next element witout advancing.
void *it_peek (It *this);

/// it_add adds an element at the end of 'this'
It *it_add(It *this, void *element);

/// it_add adds an element at the beginning of 'this'
It *it_add0(It *this, void *element);

///
It *it_cat (It *this, It *another);

///
It *it_take (It *this, size_t n);

///
It *it_takef (It *this, bool (*predicate)(void *e));

///
It *it_drop (It *this, size_t n);

///
It *it_dropf (It *this, bool (*predicate)(void *e));

///
It *it_filter (It *this, bool (*predicate)(void *e));

///
It *it_map (It *this, void *(*converter)(void *e));

/// it_map2 applies conv1 to the first element and conv2 to the others.
It *it_map2 (It *this, void *(*conv1)(void *e), void *(*conv2)(void *e));

///
It *it_zip (It *it1, It *it2);

///
It *it_zip3 (It *it1, It *it2, It *it3);

///
It *it_reverse (It *this);

///
It *it_sort (It *this, bool (*comparator)(void *e1, void *e2));

///
void it_each (It *this, void (*f)(void *e));

///
void it_each_ix (It *this, void (*f)(void *e, size_t ix));

///
size_t it_count (It *this);

///
bool it_eq (It *it1, It *it2, bool (*feq)(void *e1, void *e2));

///
int it_index (It *this, bool (*predicate)(void *e));

///
bool it_contains (It *this, bool (*predicate)(void *e));

///
int it_last_index (It *this, bool (*predicate)(void *e));

/// Returns the first element which satisfies 'predicate' or opt_null.
Opt *it_find (It *this, bool (*predicate)(void *e));

/// Returns the first element which satisfies 'predicate' or 'option'.
void *it_ofind (It *this, void *option, bool (*predicate)(void *e));

#endif


