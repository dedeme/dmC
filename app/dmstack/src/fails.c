// Copyright 29-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "fails.h"

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
    "Stack pop: Expected type token ['%s'], found type '%s'",
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
    "List %s\nExpected type of element %d: '%s', actual: '%s'",
    token_to_str(token_new_list(list)), ix,
    token_type_to_str(expected),
    token_type_to_str(token_type(arr_get(list, ix)))
  ));

}
