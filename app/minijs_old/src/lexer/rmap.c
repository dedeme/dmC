// Copyright 14-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/List.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "lexer/rmap.h"
#include "lexer/lex.h"
#include "lexer/Tx.h"
#include "lexer/rstring.h"
#include "lexer/rvalue.h"
#include "ct/Avalue.h"
#include "ct/Avatt.h"
#include "ast/Value.h"
#include "DEFS.h"

static Tx *entry (void **kv, Tx *tx) {
  Tx *r;

  Value *key;
  Value *v;

  tx = rstring(&key, tx);

  if (tx_eq(tx, r = lex_cconst(tx, ':')))
    TH(tx) "Expected ':'" _TH
  tx = lex_blanks(r);

  tx = rvalue(&v, tx);

  Avalue *vs = avalue_new();
  avalue_add(vs, key);
  avalue_add(vs, v);
  *kv = vs;

  return tx;
}

Tx *rmap(Value **v, Tx *tx) {
  Tx *r;
  Pos *pos = tx_pos(tx);

  if (tx_eq(tx, r = lex_cconst(tx, '{'))) {
    return tx;
  }
  tx = lex_blanks(r);

  List/*Avalue*/ *kvs = list_new();
  tx = lex_list(&kvs, tx, '}', (Tx *(*)(void **, Tx *))entry);

  Avalue *vs = avalue_new();
  EACHL(kvs, Avalue, kv) {
    avalue_add(vs, avalue_get(kv, 0));
    avalue_add(vs, avalue_get(kv, 1));
  }_EACH

  *v = value_new_map(pos, avatt_new(), vs);
  return tx;
}
