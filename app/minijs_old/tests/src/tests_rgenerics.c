// Copyright 2-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_rgenerics.h"
#include "lexer/rclass.h"

#define JS json_warray

void tests_rgenerics() {
  puts("Reader: generics");

  Class *c;
  Achar *generics;

  c = rclass(cpath_new("generics/F1"));
  generics = class_generics(c);
  assert(!strcmp(class_id(c), "generics_F1"));
  assert(!class_local(c));
  assert(!mchar_size(class_imports(c)));
  assert(!achar_size(generics));
  assert(type_type(class_super(c)) == VOID);
  assert(!aatt_size(class_statics(c)));
  assert(!aatt_size(class_instance(c)));
  assert(!strcmp(
    JS(class_serialize(c)),
    JS(class_serialize(class_restore(class_serialize(c))))
  ));

  c = rclass(cpath_new("generics/F2"));
  generics = class_generics(c);
  assert(!strcmp(class_id(c), "generics_F2"));
  assert(!class_local(c));
  assert(!mchar_size(class_imports(c)));
  assert(achar_size(generics) == 1);
  assert(!strcmp(achar_get(generics, 0), "A"));
  assert(type_type(class_super(c)) == VOID);
  assert(!aatt_size(class_statics(c)));
  assert(!aatt_size(class_instance(c)));
  assert(!strcmp(
    JS(class_serialize(c)),
    JS(class_serialize(class_restore(class_serialize(c))))
  ));

  c = rclass(cpath_new("generics/F3"));
  generics = class_generics(c);
  assert(!strcmp(class_id(c), "generics_F3"));
  assert(!class_local(c));
  assert(!mchar_size(class_imports(c)));
  assert(achar_size(generics) == 3);
  assert(!strcmp(achar_get(generics, 0), "A"));
  assert(!strcmp(achar_get(generics, 1), "B"));
  assert(!strcmp(achar_get(generics, 2), "C"));
  assert(type_type(class_super(c)) == VOID);
  assert(!aatt_size(class_statics(c)));
  assert(!aatt_size(class_instance(c)));
  assert(!strcmp(
    JS(class_serialize(c)),
    JS(class_serialize(class_restore(class_serialize(c))))
  ));

  puts("    Finished");
}
