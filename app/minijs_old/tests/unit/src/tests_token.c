// Copyright 24-Mar-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_token.h"
#include "lexer/token.h"
#include "lexer/Txpos.h"

static Txpos *mk_tx(char *t) {
  Cpath *path = cpath_new("Test1");
  return txpos_new(path, t, t, t + strlen(t), 1, 0);
}

void tests_token() {
  printf("token test\n");

  Txpos *tx = mk_tx("");
  Txpos *r = token_skip_stat(tx);
  assert(txpos_eq(tx, r));

  tx = mk_tx(";");
  r = token_skip_stat(tx);
  assert(txpos_nchar(r) == 1);

  tx = mk_tx("ab;");
  r = token_skip_stat(tx);
  assert(txpos_nchar(r) == 3);

  tx = mk_tx("//axdfrttg;\nab;");
  r = token_skip_stat(tx);
  assert(txpos_nline(r) == 2);
  assert(txpos_nchar(r) == 3);

  tx = mk_tx("a/*ññ*/ab;");
  r = token_skip_stat(tx);
  assert(txpos_nchar(r) == 10);

  tx = mk_tx("a/* ñ ñ */a  b ;");
  r = token_skip_stat(tx);
  assert(txpos_nchar(r) == 16);

  tx = mk_tx("a/* ñ ñ */a  b ");
  r = token_skip_stat(tx);
  assert(txpos_nchar(r) == 15);

  tx = mk_tx("a/* ñ ñ */a {a  ;{c;}} }}}  b ;");
  r = token_skip_stat(tx);
  assert(txpos_nchar(r) == 31);

  printf( "    Finished\n");
}
