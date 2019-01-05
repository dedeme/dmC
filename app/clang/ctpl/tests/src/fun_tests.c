// Copyright 23-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "fun_tests.h"

#include "fun_tests.h"
#include <assert.h>
#include "Fun.h"

void fun_tests(void) {
  puts("Fun tests:");

  Fun *fun;
  char *error;

  fun_from_str(&error, &fun, "- void (* proc  )(void *, char *) : _proc");
  assert(!error);
  assert(fun_gstype(fun) == fun_NONE);
  assert(str_eq(fun_id(fun), "proc"));
  assert(str_eq(fun_template(fun), "void (*$)(void *, char *)"));
  assert(str_eq(fun_value(fun), "_proc"));
  fun_free(fun);

  fun_from_str(&error, &fun, "void (* proc  )(void *, char *) : _proc");
  assert(!error);
  assert(fun_gstype(fun) == fun_GETTER);
  assert(str_eq(fun_id(fun), "proc"));
  assert(str_eq(fun_template(fun), "void (*$)(void *, char *)"));
  assert(str_eq(fun_value(fun), "_proc"));

  StName *stName;
  stName_from_str(&error, &stName, "Person : serial");
  Buf *bf = buf_new();
  fun_w_struct(fun, bf);
  assert(str_ends(buf_str(bf), "  void (*proc)(void *, char *);\n"));
  fun_w_constructor(fun, bf);
  assert(str_ends(
    buf_str(bf), "  void (*this->proc)(void *, char *) = _proc;\n"
  ));
  fun_w_get(fun, bf, stName);
  assert(str_ends(
    buf_str(bf),
    "void (*person_proc(Person *this))(void *, char *) "
    "{\n  return this->proc;\n}\n"
  ));
  fun_wh_get(fun, bf, stName);
  assert(str_ends(
    buf_str(bf), "void (*person_proc(Person *this))(void *, char *);\n"
  ));

  buf_free(bf);
  stName_free(stName);
  fun_free(fun);
  puts("    Finished");
}

