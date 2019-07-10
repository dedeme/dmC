// Copyright 09-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "book.h"
#include "tpls/book_index.h"
#include "tpls/book_geany.h"
#include "tpls/book_js.h"

void book (char *name) {
  if (file_exists(name)) {
    printf("'%s' already exists\n", name);
    return;
  }
  file_mkdir(name);
  file_mkdir(path_cat(name, "exercises", NULL));
  file_write(path_cat(name, "index.html", NULL), book_index());
  file_write(
    path_cat( name, str_f("%s.geany", name), NULL), book_geany(name)
  );
  file_write(path_cat(name, "book.js", NULL), book_js());
  printf("Book '%s' created\n", name);
}
