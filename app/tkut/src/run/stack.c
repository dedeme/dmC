// Copyright 02-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "run/stack.h"

struct stack_entry {
  char *module_name;
  int nline;
  int nchar;
};

static struct stack_entry *new_entry (char *module_name, int nline, int nchar) {
  struct stack_entry *this = MALLOC(struct stack_entry);
  this->module_name = module_name;
  this->nline = nline;
  this->nchar = nchar;
  return this;
}

struct run_stack {
  struct stack_entry *head;
  Stack *tail;
};

Stack *stack_new (void) {
  Stack *this = MALLOC(Stack);
  this->head = NULL;
  this->tail = NULL;
  return this;
}

/// returns a new stack adding entry to 'stack'.
Stack *stack_add(Stack *stack, char *module_name, int nline, int nchar) {
  Stack *this = MALLOC(Stack);
  this->head = new_entry(module_name, nline, nchar);
  this->tail = stack;
  return this;
}

/// Returns stack as Arr<char> (from newer to older)
Arr *stack_to_arr(Stack *this) {
  // <char>
  Arr *r = arr_new();
  while (this->head) {
    struct stack_entry *e = this->head;
    arr_push(r, str_f(
      "%s.kut:%d:%d:",
      e->module_name, e->nline, e->nchar
    ));
    this = this->tail;
  }
  return r;
}
