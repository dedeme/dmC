// Copyright 2-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_rempty.h"
#include "lexer/rclass.h"

#define JS json_warray

void tests_rempty() {
  puts("Reader: empty file");

  Class *c = rclass(cpath_new("Empty"));
  assert(!strcmp(class_id(c), "Empty"));
  assert(!class_local(c));
  assert(!mchar_size(class_imports(c)));
  assert(!achar_size(class_generics(c)));
  assert(type_type(class_super(c)) == VOID);
  assert(!aatt_size(class_statics(c)));
  assert(!aatt_size(class_instance(c)));
  assert(!strcmp(
    JS(class_serialize(c)),
    JS(class_serialize(class_restore(class_serialize(c))))
  ));

  c = rclass(cpath_new("sub/Empty"));
  assert(!strcmp(class_id(c), "sub_Empty"));
  assert(!class_local(c));
  assert(!mchar_size(class_imports(c)));
  assert(!achar_size(class_generics(c)));
  assert(type_type(class_super(c)) == VOID);
  assert(!aatt_size(class_statics(c)));
  assert(!aatt_size(class_instance(c)));
  assert(!strcmp(
    JS(class_serialize(c)),
    JS(class_serialize(class_restore(class_serialize(c))))
  ));

  c = rclass(cpath_new("local/Local"));
  assert(!strcmp(class_id(c), "local_Local"));
  assert(class_local(c));
  assert(!mchar_size(class_imports(c)));
  assert(!achar_size(class_generics(c)));
  assert(type_type(class_super(c)) == VOID);
  assert(!aatt_size(class_statics(c)));
  assert(!aatt_size(class_instance(c)));
  assert(!strcmp(
    JS(class_serialize(c)),
    JS(class_serialize(class_restore(class_serialize(c))))
  ));

  puts("    Finished");
}
