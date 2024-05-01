// Copyright 02-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Stack trace.

#ifndef RUN_STACK_H
  #define RUN_STACK_H

#include "kut/arr.h"

///
typedef struct run_stack Stack;

/// Constructor.
Stack *stack_new (void);

/// returns a new stack adding entry to 'stack'.
Stack *stack_add(Stack *stack, char *module_name, int nline, int nchar);

/// Returns stack as Arr<char> (from newer to older)
Arr *stack_to_arr(Stack *this);

#endif
