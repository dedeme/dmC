// Copyright 3-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/rtype.h"
#include "lexer/token.h"
#include "ast/Atype.h"
#include "DEFS.h"

static Tx *rtp(Type **type, Tx *tx) {
  Type *tp;
  Tx *r;
  char *id;

  if (tx_neq(tx, r = token_cconst(tx, '('))) {
    Atype *params;
    r = token_list(&params, r, ':', (Tx *(*)(void **, Tx *))rtp);
    tx = r;

    if (tx_neq(tx, r = token_cconst(tx, ')'))) {
      atype_add(params, type_new_void());
    } else {
      tx = rtp(&tp, tx);
      atype_add(params, tp);
      if (tx_eq(tx, r = token_cconst(tx, ')')))
        TH(tx) "Expected ')'" _TH
    }

    tp = type_new_fn(params);
  } else if (tx_neq(tx, r = token_cconst(tx, '['))) {
    tx = r;

    tx = rtp(&tp, tx);
    if (tx_eq(tx, r = token_cconst(tx, ']')))
      TH(tx) "Expected ']'" _TH

    tp = type_new_arr(tp);
  } else if (tx_neq(tx, r = token_cconst(tx, '{'))) {
    tx = r;

    tx = rtp(&tp, tx);
    if (tx_eq(tx, r = token_cconst(tx, '}')))
      TH(tx) "Expected '}'" _TH

    tp = type_new_map(tp);
  } else if (tx_neq(tx, r = token_cconst(tx, '*'))) {
    tp = type_new_any();
  } else if (tx_neq(tx, r = token_valid_id(&id, tx))) {
    tx = r;

    if (tx_neq(tx, r = token_cconst(tx, '<'))) {
      tx = r;

      Atype *params;
      r = token_list(&params, r, '>', (Tx *(*)(void **, Tx *))rtp);
      if (arr_size(params) == 0)
        TH(tx) "Expected at least one parameter" _TH

      tp = type_new_data(id, params);
    } else {
      tp = type_new_data(id, atype_new());
    }
  } else {
    TH(tx) "Wrong type definition" _TH
  }

  *type = tp;
  return r;
}

Tx *rtype(Type **type, Tx *tx) {
  Tx *r;
  if (tx_eq(tx, r = token_cconst(tx, ':'))) {
    *type = NULL;
    return tx;
  }

  return rtp(type, r);
}
