// Copyright 06-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/attachs.h"
#include "lexer/token.h"
#include "lexer/values.h"

Txpos *attachs_read_dot(Arr/*Attach*/ *atts, Txpos *tx) {
  Txpos *r;

  if (txpos_eq(tx, r = token_cconst(tx, '.'))) {
    return tx;
  }
  tx = r;

  char *id;
  if (txpos_eq(tx, r = token_id(&id, tx)))
    TH(tx) "Expected an identifier" _TH
  tx = r;

  arr_add(atts, attach_new_dot(id));
  return attachs_read_all(atts, tx);
}

/// Read an Attach dot or sub and adds it to 'atts'.
Txpos *attachs_read_dot_sub(Arr/*Attach*/ *atts, Txpos *tx) {
  Txpos *r;

  if (txpos_neq(tx, r = attachs_read_dot(atts, tx))){
    tx = r;
    return tx;
  }

  if (txpos_eq(tx, r = token_cconst(tx, '['))) {
    return tx;
  }
  tx = r;

  Value *val;
  tx = values_read(&val, tx, false);

  if (txpos_eq(tx, r = token_cconst(tx, ']')))
    TH(tx) "Expected ']'" _TH
  tx = r;

  arr_add(atts, attach_new_sub(val));
  return attachs_read_all(atts, tx);
}

static Txpos *read_value(void **value, Txpos *tx) {
  return values_read((Value **)value, tx, false);
}

/// Read an Attach dot, sub or fn and adds it to 'atts'.
Txpos *attachs_read_all(Arr/*Attach*/ *atts, Txpos *tx) {
  Txpos *r;

  if (txpos_neq(tx, r = attachs_read_dot(atts, tx))){
    tx = r;
    return tx;
  }

  if (txpos_neq(tx, r = attachs_read_dot_sub(atts, tx))){
    tx = r;
    return tx;
  }

  if (txpos_eq(tx, r = token_cconst(tx, '('))) {
    return tx;
  }
  tx = r;

  Arr/*Value*/ *vs;
  tx = token_list(&vs, tx, ')', read_value);

  arr_add(atts, attach_new_fn(vs));
  return attachs_read_all(atts, tx);
}

