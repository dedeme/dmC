// Copyright 27-Mar-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_numbers.h"
#include "lexer/numbers.h"
#include "lexer/Txpos.h"

static Txpos *mk_tx(char *t) {
  Cpath *path = cpath_new("Test1");
  return txpos_new(path, t, t, t + strlen(t), 1, 0);
}

void tests_numbers() {
  printf("numbers test\n");

  Txpos *tx = mk_tx("");
  Txpos *r = numbers_skip(tx);
  assert(txpos_start(r) == txpos_end(r));
  assert(txpos_nchar(r) == 0);

  tx = mk_tx(".");
  r = numbers_skip(tx);
  assert(txpos_nchar(r) == 1);

  tx = mk_tx("b");
  r = numbers_skip(tx);
  assert(txpos_nchar(r) == 0);

  tx = mk_tx(".b");
  r = numbers_skip(tx);
  assert(txpos_nchar(r) == 1);

  tx = mk_tx("7e");
  r = numbers_skip(tx);
  assert(txpos_nchar(r) == 1);

  tx = mk_tx("7e+4");
  r = numbers_skip(tx);
  assert(txpos_nchar(r) == 4);

  tx = mk_tx("1_245.2e5");
  r = numbers_skip(tx);
  assert(txpos_nchar(r) == 9);

  tx = mk_tx("1_245.2_e5");
  r = numbers_skip(tx);
  assert(txpos_nchar(r) == 7);

  tx = mk_tx("1_245.2e_5");
  r = numbers_skip(tx);
  assert(txpos_nchar(r) == 7);

  tx = mk_tx("1_245.2e5_");
  r = numbers_skip(tx);
  assert(txpos_nchar(r) == 9);

  tx = mk_tx("1_245.2.e5");
  r = numbers_skip(tx);
  assert(txpos_nchar(r) == 7);


  printf( "    Finished\n");
}
