// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_imported.h"
#include "ast/Imported.h"

void tests_imported() {
  puts("Imported");

  Imported *im = imported_get();
  imported_add(im, class_new("a"));
  assert(imported__class(im, "a"));
  assert(!imported__class(im, "b"));

  RANGE0(i, 150) {
    imported_add(im, class_new(str_printf("%d", i)));
  }_RANGE

  assert(imported__class(im, "110"));
  assert(!imported__class(im, "a"));

  puts("    Finished");
}
