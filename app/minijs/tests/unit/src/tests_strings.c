// Copyright 24-Mar-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_strings.h"
#include "lexer/strings.h"
#include "lexer/Txpos.h"

static Txpos *mk_tx(char *t) {
  Cpath *path = cpath_new("Test1");
  return txpos_new(path, t, t, t + strlen(t), 1, 0);
}

void tests_strings() {
  printf("strings test\n");

  Txpos *tx = mk_tx("\"");
  Txpos *r = strings_skip(tx, '"');
  assert(txpos_nchar(r) == 1);

  tx = mk_tx("\"   ");
  r = strings_skip(tx, '"');
  assert(txpos_nchar(r) == 4);

  tx = mk_tx("abc\"   ");
  r = strings_skip(tx, '"');
  assert(txpos_nchar(r) == 7);

  tx = mk_tx("`");
  r = strings_skip(tx, '`');
  assert(txpos_nchar(r) == 1);

  tx = mk_tx("`   ");
  r = strings_skip(tx, '`');
  assert(txpos_nchar(r) == 4);

  tx = mk_tx("abc`   ");
  r = strings_skip(tx, '`');
  assert(txpos_nchar(r) == 7);

  tx = mk_tx("ab\nc`   ");
  r = strings_skip(tx, '`');
  assert(txpos_nchar(r) == 5);
  assert(txpos_nline(r) == 1);

  tx = mk_tx("ñ'");
  r = strings_skip(tx, '\'');
  assert(txpos_nchar(r) == 2);

  tx = mk_tx(" '   ");
  r = strings_skip(tx, '\'');
  assert(txpos_nchar(r) == 5);


  printf( "    Finished\n");
}
