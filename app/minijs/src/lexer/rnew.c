// Copyright 12-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/rnew.h"
#include "lexer/token.h"
#include "lexer/rstat.h"
#include "lexer/rblock.h"
#include "ast/Avalue.h"
#include "ast/Astat.h"
#include "DEFS.h"

static Tx *new_id(char **id, Tx *tx) {
  Tx *r;

  Buf *bf = buf_new();

  if (tx_neq(tx, r = token_cconst0(tx, '@'))) {
    tx = r;
    buf_cadd(bf, '@');
  }

  char ch;
  if (tx_neq(tx, r = token_csplit(&ch, tx, "+>"))) {
    tx = r;
    buf_cadd(bf, ch);
  }

  char *i;
  if (tx_eq(tx, r = token_valid_id(&i, tx)))
    TH(tx) "Expected an identifier" _TH
  tx = r;

  buf_add(bf, i);
  *id = buf_to_str(bf);
  return tx;
}

Tx *rnew(Value **value, Tx *tx, Class *c, Type *tp, bool is_public) {
  Tx *r;
  Tx *start = tx;
  Pos *pos = tx_pos(tx);

  Atype *params = type_params(tp);
  Achar *new_ids;
  tx = token_fn_list(&new_ids, tx, new_id);

  if (arr_size(params) != achar_size(new_ids))
    TH(start)
      "Type definition requires '%d' paramters but function has '%d'",
      arr_size(params), achar_size(new_ids)
    _TH

  Achar *ids = achar_new();
  EACH(params, Type, t) {
    char *id = achar_get(new_ids, _i);
    switch (*id) {
      case '+':
        ++id;
        achar_add(ids, id);
        aatt_add(
          class_instance(c),
          att_new(true, id, t, false, value_new_null(pos))
        );
        break;
      case '@':
        ++id;
        switch (*id) {
          case '+':
            ++id;
            achar_add(ids, id);
            aatt_add(
              class_instance(c),
              att_new(false, id, t, false, value_new_null(pos))
            );
            break;
          default:
            achar_add(ids, id);
            aatt_add(
              class_instance(c),
              att_new(false, id, t, true, value_new_null(pos))
            );
        }
        break;
      case '>':
        ++id;
        achar_add(ids, id);
        break;
      default:
        achar_add(ids, id);
        aatt_add(
          class_instance(c),
          att_new(true, id, t, true, value_new_null(pos))
        );
    }
  }_EACH

  int public = true;
  Stat *st;
  for (;;) {
    if (tx_at_end(tx))
      TH(start) "Function 'new' without '_code'" _TH

    if (tx_neq(tx, r = token_directive(tx, "_private"))) {
      tx = r;
      public = false;
      continue;
    }

    if (tx_neq(tx, r = token_directive(tx, "_code"))) {
      tx = r;
      break;
    }

    tx = rstat(&st, tx);
    if (stat_type(st) != SVAL && stat_type(st) != SVAR)
      TH(tx) "Expected 'val' or 'var' declaration" _TH

    char *id = stat_id(st);
    if (class_contains_id(c, id))
      TH(tx) "Identifier '%s' is duplicated", id _TH

    Value *v = avalue_get(stat_values(st), 0);
    if (type_is_unknown(value_type(v)))
      TH(tx) "Type declaration is needed" _TH

    aatt_add(
      class_instance(c),
      att_new(public, id, value_type(v), stat_type(st) != SVAL, v)
    );
  }

  Astat *bl;
  tx = rmblock(&bl, tx);

  *value = value_new_fn(pos, ids, bl);
  return tx;
}
