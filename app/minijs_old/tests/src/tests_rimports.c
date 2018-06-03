// Copyright 2-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_rimports.h"
#include "lexer/rclass.h"

#define JS json_warray

void tests_rimports() {
  puts("Reader: imports");

  Class *c;
  Mchar *imports;

  c = rclass(cpath_new("sub/Import"));
  imports = class_imports(c);
  assert(!strcmp(class_id(c), "sub_Import"));
  assert(class_local(c));
  assert(mchar_size(imports) == 3);
  assert(!strcmp(mchar_get(imports, "Empty"), "sub_Empty"));
  assert(!strcmp(mchar_get(imports, "Emp"), "Empty"));
  assert(!strcmp(mchar_get(imports, "Local"), "local_Local"));
  assert(!achar_size(class_generics(c)));
  assert(type_type(class_super(c)) == VOID);
  assert(!aatt_size(class_statics(c)));
  assert(!aatt_size(class_instance(c)));
  assert(!strcmp(
    JS(class_serialize(c)),
    JS(class_serialize(class_restore(class_serialize(c))))
  ));

  c = rclass(cpath_new("imports/F1"));
  imports = class_imports(c);
  assert(!strcmp(class_id(c), "imports_F1"));
  assert(!class_local(c));
  assert(mchar_size(imports) == 3);
  assert(!strcmp(mchar_get(imports, "Empty"), "Empty"));
  assert(!strcmp(mchar_get(imports, "Import"), "sub_Import"));
  assert(!strcmp(mchar_get(imports, "L"), "local_Local"));
  assert(!achar_size(class_generics(c)));
  assert(type_type(class_super(c)) == VOID);
  assert(!aatt_size(class_statics(c)));
  assert(!aatt_size(class_instance(c)));
  assert(!strcmp(
    JS(class_serialize(c)),
    JS(class_serialize(class_restore(class_serialize(c))))
  ));

  c = rclass(cpath_new("imports/F2"));
  imports = class_imports(c);
  assert(!strcmp(class_id(c), "imports_F2"));
  assert(!class_local(c));
  assert(mchar_size(imports) == 3);
  assert(!strcmp(mchar_get(imports, "Empty"), "sub_Empty"));
  assert(!strcmp(mchar_get(imports, "Import"), "sub_Import"));
  assert(!strcmp(mchar_get(imports, "Local"), "local_Local"));
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
