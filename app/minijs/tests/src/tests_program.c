// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_program.h"
#include "ast/Program.h"

void tests_program() {
  puts("Program");

  Program *prg = program_get();
  program_add(prg, class_new("a"));
  assert(program__class(prg, "a"));
  assert(!program__class(prg, "b"));

  RANGE0(i, 150) {
    program_add(prg, class_new(str_printf("%d", i)));
  }_RANGE

  assert(program__class(prg, "110"));
  assert(program__class(prg, "a"));
  assert(!program__class(prg, "b"));
  assert(arr_size(prg) == 151);

  puts("    Finished");
}
