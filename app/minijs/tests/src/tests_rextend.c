// Copyright 2-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_rextend.h"
#include "lexer/rclass.h"

#define JS json_warray

void tests_rextend() {
  puts("Reader: extend");

  Class *c;
  Mchar *imports;
  Achar *generics;
  Achar *super;

  c = rclass(cpath_new("extend/F1"));
  imports = class_imports(c);
  super = class_super(c);
  assert(!strcmp(class_id(c), "extend_F1"));
  assert(!class_local(c));
  assert(!arr_size(imports));
  assert(!arr_size(class_generics(c)));
  assert(arr_size(super) == 1);
  assert(!strcmp("Abc", achar_get(super, 0)));
  assert(!arr_size(class_statics(c)));
  assert(!arr_size(class_instance(c)));
  assert(!strcmp(
    JS(class_serialize(c)),
    JS(class_serialize(class_restore(class_serialize(c))))
  ));

  c = rclass(cpath_new("extend/F2"));
  imports = class_imports(c);
  super = class_super(c);
  assert(!strcmp(class_id(c), "extend_F2"));
  assert(class_local(c));
  assert(!arr_size(imports));
  assert(!arr_size(class_generics(c)));
  assert(arr_size(super) == 1);
  assert(!strcmp("Abc", achar_get(super, 0)));
  assert(!arr_size(class_statics(c)));
  assert(!arr_size(class_instance(c)));
  assert(!strcmp(
    JS(class_serialize(c)),
    JS(class_serialize(class_restore(class_serialize(c))))
  ));

  c = rclass(cpath_new("extend/F3"));
  imports = class_imports(c);
  generics = class_generics(c);
  super = class_super(c);
  assert(!strcmp(class_id(c), "extend_F3"));
  assert(!class_local(c));
  assert(arr_size(generics) == 1);
  assert(!strcmp("A", achar_get(generics, 0)));
  assert(!arr_size(imports));
  assert(arr_size(super) == 3);
  assert(!strcmp("Abc", achar_get(super, 0)));
  assert(!strcmp("A", achar_get(super, 1)));
  assert(!strcmp("B", achar_get(super, 2)));
  assert(!arr_size(class_statics(c)));
  assert(!arr_size(class_instance(c)));
  assert(!strcmp(
    JS(class_serialize(c)),
    JS(class_serialize(class_restore(class_serialize(c))))
  ));

  c = rclass(cpath_new("extend/F4"));
  imports = class_imports(c);
  generics = class_generics(c);
  super = class_super(c);
  assert(!strcmp(class_id(c), "extend_F4"));
  assert(!class_local(c));
  assert(arr_size(generics) == 1);
  assert(!strcmp("A", achar_get(generics, 0)));
  assert(arr_size(imports) == 2);
  assert(!strcmp("Empty", mchar_get(imports, "Empty")));
  assert(!strcmp("local_Local", mchar_get(imports, "B")));
  assert(arr_size(super) == 3);
  assert(!strcmp("Abc", achar_get(super, 0)));
  assert(!strcmp("A", achar_get(super, 1)));
  assert(!strcmp("B", achar_get(super, 2)));
  assert(!arr_size(class_statics(c)));
  assert(!arr_size(class_instance(c)));
  assert(!strcmp(
    JS(class_serialize(c)),
    JS(class_serialize(class_restore(class_serialize(c))))
  ));

  puts("    Finished");
}
