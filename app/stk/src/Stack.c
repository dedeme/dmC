// Copyright 18-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Stack.h"

/* .
-Stack
  -value: void *: _i
  -next: Stack *: _i
*/

struct stack_Stack {
  void *value;
  Stack *next;
};

Stack *_stack_new(void *value, Stack *next) {
  Stack *this = malloc(sizeof(Stack));
  this->value = value;
  this->next = next;
  return this;
}

void stack_free(Stack *this, void(*ffree)(void *)) {
  if (this) {
    stack_clear(this, ffree);
    free(this);
  }
};

Stack *stack_new (void) {
  return _stack_new(NULL, NULL);
}

void stack_push (Stack *this, void *value_new) {
  Stack *st = _stack_new(this->value, this->next);
  this->value = value_new;
  this->next = st;
}

void *stack_pop_new (Stack *this) {
  if (!this->next) FAIL("Stack is empty");

  Stack *st = this->next;
  void *r = this->value;
  this->next = st->next;
  this->value = st->value;
  free(st);
  return r;
}

void stack_pop_del (Stack *this, void(*ffree)(void *)) {
  if (!this->next) FAIL("Stack is empty");

  if (this->next) {
    ffree(this->value);
    Stack *next = this->next;
    this->next = next->next;
    this->value = next->value;
    free(next);
  }
}

void *stack_peek (Stack *this) {
  if (!this->next) FAIL("Stack is empty");

  return this->value;
}

Stack *stack_get_null (Stack *this, int ix) {
  if (ix < 0) FAIL(str_f_new("'%d' is less than '0'", ix))

  REPEAT(ix)
    if (this->next) {
      this = this->next;
    }
  _REPEAT

  return this->next ? this : NULL;
}

void *stack_peek_n (Stack *this, int ix) {
  Stack *r = stack_get_null(this, ix);

  if (!r) FAIL(str_f_new("Stack has less than %d elements", ix))

  return r->value;
}

int stack_empty (Stack *this) {
  return !this->next;
}

int stack_has_element (Stack *this, int ix) {
  return stack_get_null(this, ix) != NULL;
}

int stack_count (Stack *this) {
  int r = 0;
  while (this->next) {
    ++r;
    this = this->next;
  }
  return r;
}

void stack_del (Stack *this, int ix, void(*ffree)(void *)) {
  Stack *st = stack_get_null(this, ix);

  if (!st) FAIL(str_f_new("Stack has less than %d elements", ix))

  ffree(st->value);
  Stack *next = st->next;
  st->next = next->next;
  st->value = next->value;
  free(next);
}

void stack_del_range (Stack *this, int begin, int end, void(*ffree)(void *)) {
  Stack *st = stack_get_null(this, begin);
  RANGE(i, begin, end)
    if (!st) FAIL(str_f_new("Stack has less than %d elements", i))

    ffree(st->value);
    Stack *next = st->next;
    st->next = next->next;
    st->value = next->value;
    free(next);
  _RANGE
}

void *stack_remove_new (Stack *this, int ix) {
  Stack *st = stack_get_null(this, ix);

  if (!st) FAIL(str_f_new("Stack has less than %d elements", ix))

  void *r = st->value;
  Stack *next = st->next;
  st->next = next->next;
  st->value = next->value;
  free(next);
  return r;
}

/// Removes elements from begin (inclusive) to end (exclusive) and returns them
Arr *stack_remove_range_new (
  Stack *this,
  int begin,
  int end,
  void(*ffree)(void *)
) {
  Arr *r = arr_new(ffree);

  Stack *st = stack_get_null(this, begin);
  RANGE(i, begin, end)
    if (!st) FAIL(str_f_new("Stack has less than %d elements", i))

    arr_push(r, st->value);
    Stack *next = st->next;
    st->next = next->next;
    st->value = next->value;
    free(next);
  _RANGE

  return r;
}

void stack_clear_n (Stack *this, int n, void(*ffree)(void *)) {
  if (n > 0) {
    REPEAT(n)
      if (this->next) {
        ffree(this->value);
        Stack *next = this->next;
        this->next = next->next;
        this->value = next->value;
        free(next);
      }
    _REPEAT
  }
}

void stack_clear (Stack *this, void(*ffree)(void *)) {
  while (this->next) {
    ffree(this->value);
    Stack *next = this->next;
    this->next = next->next;
    this->value = next->value;
    free(next);
  }
}
