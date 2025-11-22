// Copyright 12-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "runner/stack.h"
#include "fileix.h"

struct stack_Stack {
  StatCode *stat;
  Stack *stack;
};

static Stack root = { .stat = NULL, .stack = NULL };

Stack *stack_new (void) {
  return &root;
}

Stack *stack_add (Stack *this, StatCode *stat) {
  Stack *r = MALLOC(Stack);
  r->stat = stat;
  r->stack = this;
  return r;
}

Arr *stack_to_arr (Stack *this) {
  Arr *r = arr_new();
  int i = 0;
  while (i < 5 && this != &root) {
    StatCode *st = this->stat;
    this = this->stack;
    arr_push(r, str_f(
      "%s:%d:",
      fileix_to_str(stat_code_file_ix(st)),
      stat_code_line(st)
    ));
    ++i;
  }
  return r;
}

