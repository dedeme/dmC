// Copyright 18-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef STACK_H
  #define STACK_H

#include "dmc/std.h"

///
typedef struct stack_Stack Stack;

///
void stack_free(Stack *this, void(*ffree)(void *));

/// Returns a new empty stack
Stack *stack_new (void);

/// Adds a value. The value rest in postion 0.
void stack_push (Stack *this, void *value_new);

/// Returns and removes the element 0
void *stack_pop_new (Stack *this);

/// Deletes the element 0
void stack_pop_del (Stack *this, void(*ffree)(void *));

/// Returns the element 'ix' or NULL if it does not exist. First element
/// has 'ix' 0
Stack *stack_get_null (Stack *this, int ix);

/// Equals to stack_peekn(this, 0)
void *stack_peek (Stack *this);

/// Returns the element 'ix'. . First element has 'ix' 0.
void *stack_peek_n (Stack *this, int ix);

/// Returns 1 if 'this' is empty
int stack_empty (Stack *this);

/// Returns 1 if the element 'ix' exists. First element has 'ix' 0
int stack_has_element (Stack *this, int ix);

/// Returns the elements number of 'this'. It is O(n).
int stack_count (Stack *this);

/// Deletes the element ix. First element has 'ix' 0
void stack_del (Stack *this, int ix, void(*ffree)(void *));

/// Deletes elements from begin (inclusive) to end (exclusive)
void stack_del_range (Stack *this, int begin, int end, void(*ffree)(void *));

/// Removes the element ix and returns it. First element has 'ix' 0
void *stack_remove_new (Stack *this, int ix);

/// Removes elements from begin (inclusive) to end (exclusive) and returns.
/// them. For example: Remove range 1-3 of (9, 10, 11, 12, 13, 14) returns
/// [13, 12, 11] and lets (9, 10, 14) in stack.
Arr *stack_remove_range_new (
  Stack *this,
  int begin,
  int end,
  void(*ffree)(void *)
);

/// Deletes the first 'n' elements. If 'this' have less elements than 'n'
/// removes all.
void stack_clear_n (Stack *this, int n, void(*ffree)(void *));

/// Deletes all elements
void stack_clear (Stack *this, void(*ffree)(void *));

#endif
