// Copyright 29-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "fails.h"
#include <signal.h>

struct fails_Mentry {
  pthread_t t;
  Machine *m;
};
struct fails_Mentry *fails_Mentry_new (pthread_t t, Machine *m) {
  struct fails_Mentry *this = MALLOC(struct fails_Mentry);
  this->t = t;
  this->m = m;
  return this;
}

// Arr<struct fails_Mentry
Arr *current_machines = NULL;
volatile sig_atomic_t fatal_error_in_progress = 0;

static void signal_handler (int sig) {
  if (fatal_error_in_progress)
    raise (sig);
  fatal_error_in_progress = 1;

  pthread_t th = pthread_self();
  int fn (struct fails_Mentry *e) { return pthread_equal(e->t, th); }
  struct fails_Mentry *e = opt_nget(
    it_find(it_from(current_machines), (FPRED)fn)
  );
  if (e) {
    Machine *current_machine = e->m;
    if (current_machine)
      machine_fail(current_machine, str_f("From C: %s", strsignal(sig)));
  }

  signal (sig, SIG_DFL);
  raise (sig);
}

void fails_register_machine (Machine *m) {
  arr_push(current_machines, fails_Mentry_new(pthread_self(), m));
}

void fails_unregister_machine (void) {
  pthread_t th = pthread_self();
  int fn (struct fails_Mentry *e) { return !pthread_equal(e->t, th); }
  arr_filter_in(current_machines, (FPRED)fn);
}

void fails_init (void) {
  current_machines = arr_new();
  signal(SIGFPE, signal_handler);
}

void fails_type (Machine *m, enum token_Type type) {
  machine_fail(m, str_f(
    "Stack pop: Expected token of type '%s', found type '%s'",
    token_type_to_str(type),
    token_type_to_str(token_type(arr_peek(machine_stack(m))))
  ));
}

void fails_types (Machine *m, int n, enum token_Type *types) {
  // Arr<char>
  Arr *ts = arr_new();
  RANGE0(i, n) {
    arr_push(ts, token_type_to_str(types[i]));
  }_RANGE
  machine_fail(m, str_f(
    "Stack pop: Expected token of type ['%s'], found type '%s'",
    str_join(ts, ", "),
    token_type_to_str(token_type(arr_peek(machine_stack(m))))
  ));
}

void fails_size_list (Machine *m, Arr *list, int expected) {
  machine_fail(m, str_f(
    "List %s\nExpected size: %d, actual size: %d",
    token_to_str(token_new_list(list)), expected, arr_size(list)
  ));
}

void fails_type_list (Machine *m, Arr *list, int ix, enum token_Type expected) {
  machine_fail(m, str_f(
    "List %s\nExpected element of type %d: '%s', actual: '%s'",
    token_to_str(token_new_list(list)), ix,
    token_type_to_str(expected),
    token_type_to_str(token_type(arr_get(list, ix)))
  ));
}

void fails_range (Machine *m, int min, int max, int ix) {
  machine_fail(m, str_f(
    "Index %d out of range [%d - %d], both inclusive.", ix, min, max
  ));
}

void fails_check_range (Machine *m, int min, int max, int ix) {
  if (ix < min || ix > max) fails_range(m, min, max, ix);
}
