// Copyright 2-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_rempty.h"
#include "lexer/rclass.h"

void tests_rempty() {
  puts("Reader: empty file");

  Class *c = rclass(cpath_new("Empty"));
  assert(!strcmp(class_id(c), "Empty"));
  assert(!class_local(c));
  assert(!arr_size(class_imports(c)));
  assert(!arr_size(class_generics(c)));
  assert(arr_size(class_super(c)) == 0);
  assert(!arr_size(class_statics(c)));
  assert(!arr_size(class_instance(c)));
  assert(!strcmp(
    class_serialize(c),
    class_serialize(class_restore(class_serialize(c)))
  ));

  c = rclass(cpath_new("sub/Empty"));
  assert(!strcmp(class_id(c), "sub_Empty"));
  assert(!class_local(c));
  assert(!arr_size(class_imports(c)));
  assert(!arr_size(class_generics(c)));
  assert(arr_size(class_super(c)) == 0);
  assert(!arr_size(class_statics(c)));
  assert(!arr_size(class_instance(c)));
  assert(!strcmp(
    class_serialize(c),
    class_serialize(class_restore(class_serialize(c)))
  ));

  c = rclass(cpath_new("local/Local"));
  assert(!strcmp(class_id(c), "local_Local"));
  assert(class_local(c));
  assert(!arr_size(class_imports(c)));
  assert(!arr_size(class_generics(c)));
  assert(arr_size(class_super(c)) == 0);
  assert(!arr_size(class_statics(c)));
  assert(!arr_size(class_instance(c)));
  assert(!strcmp(
    class_serialize(c),
    class_serialize(class_restore(class_serialize(c)))
  ));

  puts("    Finished");
}
