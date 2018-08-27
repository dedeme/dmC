// Copyright 06-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include "dmc/exc.h"
#include "dmc/str.h"
#include "dmc/DEFS.h"
#include "ct/Ltype.h"
#include "ast/Type.h"
#include "lexer/rtype.h"
#include "lexer/lex.h"
#include "lexer/Tx.h"
#include "DEFS.h"

Tx *rtype(Type **t, Tx *tx) {
  Type *tp;
  Tx *r;
  char *id;
  if (tx_neq(tx, r = lex_cconst(tx, '('))) {
    tx = lex_blanks(r);
    Ltype *params = ltype_new();
    tx = lex_list((List **)&params, tx, ':', (Tx *(*)(void **, Tx *))rtype);
    tx = lex_blanks(tx);

    Ltype *ret = ltype_new();
    tx = lex_list((List **)&ret, tx, ')', (Tx *(*)(void **, Tx *))rtype);

    if (ltype_empty(ret)) {
      ret = ltype_cons(ret, type_new_void());
    } else if (!ltype_empty(ret) && !ltype_empty(ltype_tail(ret))) {
      TH(tx) "More than 2 return types" _TH
    }

    tp = type_new_fn(ltype_cat(params, ret));
  } else if (tx_neq(tx, r = lex_cconst(tx, '['))) {
    tx = lex_blanks(r);

    tx = lex_blanks(rtype(&tp, tx));
    if (tx_eq(tx, r = lex_cconst(tx, ']')))
      TH(tx) "Expected ']'" _TH
    tx = r;

    Ltype *gs = ltype_new();
    gs = ltype_cons(gs, tp);
    tp = type_new_data("Arr", gs);
  } else if (tx_neq(tx, r = lex_cconst(tx, '{'))) {
    tx = lex_blanks(r);

    tx = lex_blanks(rtype(&tp, tx));
    if (tx_eq(tx, r = lex_cconst(tx, '}')))
      TH(tx) "Expected '}'" _TH
    tx = r;

    Ltype *gs = ltype_new();
    gs = ltype_cons(gs, tp);
    tp = type_new_data("Map", gs);
  } else if (tx_neq(tx, r = lex_cconst(tx, '*'))) {
    tx = r;
    tp = type_new_any();
  } else if (tx_neq(tx, r = lex_id(&id, tx))) {
    tx = lex_blanks(r);

    if (tx_neq(tx, r = lex_cconst(tx, '<'))) {
      tx = lex_blanks(r);

      Ltype *params = ltype_new();
      tx = lex_list((List **)&params, tx, '>', (Tx *(*)(void **, Tx *))rtype);
      if (ltype_empty(params))
        TH(tx) "Expected at least one parameter" _TH

      tp = type_new_data(id, params);
    } else {
      tp = type_new_data(id, ltype_new());
    }
  } else {
    TH(tx) "Wrong type definition" _TH
  }

  *t = tp;
  return tx;
}
