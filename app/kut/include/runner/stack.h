// Copyright 29-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Class of immutable 'stack trace' objects.

#ifndef RUNNER_STACK_H
  #define RUNNER_STACK_H

#include "stat.h"

///
typedef struct stack_Stack Stack;

/// Creates an empty Stack.
Stack *stack_new (void);

/// Adds a statement on front of 'this'.
Stack *stack_add (Stack *this, StatCode *stat);

/// Returns Arr<char> with the stack trace from top to bottom.
Arr *stack_to_arr (Stack *this);

#endif
