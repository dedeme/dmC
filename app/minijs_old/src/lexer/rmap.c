// Copyright 12-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/rmap.h"
#include "lexer/token.h"
#include "lexer/rstring.h"
#include "lexer/rvalue.h"
#include "ast/Avalue.h"
#include "DEFS.h"

static Tx *entry (void **kv, Tx *tx) {
  Tx *r;

  Value *key;
  Value *v;

  tx = rstring(&key, tx);

  if (tx_eq(tx, r = token_cconst(tx, ':')))
    TH(tx) "Expected ':'" _TH

  tx = rvalue(&v, r);

  Avalue *vs = avalue_new();
  avalue_add(vs, key);
  avalue_add(vs, v);
  *kv = vs;

  return tx;
}

Tx *rmap(Value **v, Tx *tx) {
  Tx *r;
  Pos *pos = tx_pos(tx);

  if (tx_eq(tx, r = token_cconst(tx, '{'))) {
    return tx;
  }
  Arr/*Avalue*/ *kvs;
  tx = token_list(&kvs, r, '}', (Tx *(*)(void **, Tx *))entry);

  Avalue *vs = avalue_new();
  EACH(kvs, Avalue, kv) {
    avalue_add(vs, avalue_get(kv, 0));
    avalue_add(vs, avalue_get(kv, 1));
  }_EACH

  *v = value_new_map(pos, arr_new(), vs);
  return tx;
}
