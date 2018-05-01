// Copyright 27-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/values.h"
#include "lexer/strings.h"
#include "lexer/numbers.h"
#include "lexer/token.h"
#include "lexer/stats.h"
#include "lexer/block.h"
#include "lexer/attachs.h"
#include "DEFS.h"

static Txpos *read_null(Value **v, Txpos *tx) {
  Txpos *r;
  if (txpos_neq(tx, r = token_const(tx, "null"))) {
    *v = value_new_null(txpos_pos(tx));
    tx = r;
  }
  return tx;
}

static Txpos *read_bool(Value **v, Txpos *tx) {
  Txpos *r;
  char *value;
  if (txpos_neq(tx, r = token_bool(&value, tx))) {
    Arr /*Attach*/ *atts = arr_new();
    tx = attachs_read_dot(atts, r);
    *v = value_new_bool(txpos_pos(tx), atts, value);
  }
  return tx;
}

static Txpos *read_byte(Value **v, Txpos *tx) {
  Txpos *r;
  char *value;
  if (txpos_neq(tx, r = numbers_byte(&value, tx))) {
    *v = value_new_byte(txpos_pos(tx), value);
    tx = r;
  }
  return tx;
}

static Txpos *read_int(Value **v, Txpos *tx) {
  Txpos *r;
  char *value;
  if (txpos_neq(tx, r = numbers_int(&value, tx))) {
    *v = value_new_int(txpos_pos(tx), value);
    tx = r;
  }
  return tx;
}

static Txpos *read_float(Value **v, Txpos *tx) {
  Txpos *r;
  char *value;
  if (txpos_neq(tx, r = numbers_float(&value, tx))) {
    *v = value_new_float(txpos_pos(tx), value);
    tx = r;
  }
  return tx;
}

static Txpos *read_string(Value **v, Txpos *tx) {
  Txpos *r;
  char *value;
  enum Vtype_t type;
  if (txpos_neq(tx, r = strings_read(&type, &value, tx))) {

    Arr /*Attach*/ *atts = arr_new();
    switch (type) {
    case VSTR:
      r = attachs_read_dot_sub(atts, r);
      *v = value_new_str(txpos_pos(tx), atts, value);
      break;
    case VCHAR:
      r = attachs_read_dot(atts, r);
      *v = value_new_char(txpos_pos(tx), atts, value);
      break;
    case VSTR2:
      r = attachs_read_dot_sub(atts, r);
      *v = value_new_str2(txpos_pos(tx), atts, value);
      break;
    default:
      THROW "'%d' unknown type" _THROW
    }

    tx = r;
  }
  return tx;
}

static Txpos *read_id(Value **v, Txpos *tx) {
  Txpos *r;
  char *value;
  if (txpos_neq(tx, r = token_valid_id(&value, tx))) {
    Arr /*Attach*/ *atts = arr_new();
    tx = attachs_read_all(atts, r);
    *v = value_new_id(txpos_pos(tx), atts, value);
  }
  return tx;
}

static Txpos *read_arr(Value **v, Txpos *tx) {
  Txpos *start = tx;
  Txpos *r;
  if (txpos_eq(tx, r = token_cconst(tx, '['))) {
    return tx;
  }
  tx = r;

  Arr/*Value*/ *list;
  Txpos *read(void **value, Txpos *tx) {
    return values_read((Value **)value, tx, false);
  }
  tx = token_list(&list, tx, ']', read);


  Arr /*Attach*/ *atts = arr_new();
  tx = attachs_read_dot_sub(atts, tx);
  *v = value_new_arr(txpos_pos(start), atts, list);

  return tx;
}

static Txpos *read_map(Value **v, Txpos *tx) {
  Txpos *start = tx;
  Txpos *r;
  if (txpos_eq(tx, r = token_cconst(tx, '{'))) {
    return tx;
  }
  tx = r;

  Arr/*Arr[Value]*/ *list;
  Txpos *read(void **value, Txpos *tx) {
    Txpos *r;
    Value *kv;
    Arr/*Value*/ *key_value = arr_new();

    if (txpos_neq(tx, r = read_string(&kv, tx))) {
      arr_add(key_value, kv);
      tx = r;
    } else
      TH(tx) "Literal map key must be a literal string" _TH

    if (txpos_eq(tx, r = token_cconst(tx, ':')))
      TH(tx) "Expected ':' (colon)" _TH
    tx = r;

    tx = values_read(&kv, tx, false);
    arr_add(key_value, kv);

    *value = (void **)key_value;
    return tx;
  }
  tx = token_list(&list, tx, '}', read);

  Arr/*Value*/ *kvs = arr_new();
  RANGE0(i, arr_size(list)) {
    Arr/*Value*/ *kv = arr_get(list, i);
    arr_add(kvs, arr_get(kv, 0));
    arr_add(kvs, arr_get(kv, 1));
  }_RANGE

  Arr /*Attach*/ *atts = arr_new();
  tx = attachs_read_dot_sub(atts, tx);
  *v = value_new_map(txpos_pos(start), atts, kvs);

  return tx;
}

static Txpos *read_with(Value **v, Txpos *tx) {
  Txpos *start = tx;
  Txpos *r;
  char *id;
  if (txpos_eq(tx, r = token_id(&id, tx))) {
    return tx;
  }
  if (strcmp(id, "with")) {
    return tx;
  }
  tx = r;

  if (txpos_eq(tx, r = token_cconst(tx, '(')))
    TH(tx) "Expected '('" _TH
  tx = r;

  Value *value;
  tx = values_read(&value, tx, true);

  Arr/*Value*/ *conditions = arr_new();
  Arr/*Value*/ *values = arr_new();
  Arr/*Value*/ *vs1 = arr_new();
  Value *v2;
  bool cont = true;
  while (cont) {
    if (txpos_eq(tx, r = token_cconst(tx, ':')))
      TH(tx) "Expected ':' (colon)" _TH
    tx = r;

    if (txpos_neq(tx, r = token_cconst(tx, '_'))) {
      arr_add(vs1, value_new_null(txpos_pos(start)));
      tx = r;
      if (txpos_eq(tx, r = token_cconst(tx, '=')))
        TH(tx) "Expected '='" _TH
      tx = r;
      cont = false;
    } else {
      /**/Txpos *read(void **value, Txpos *tx) {
      /**/  return values_read((Value **)value, tx, false);
      /**/}
      tx = token_list(&vs1, tx, '=', read);
      if (!arr_size(vs1))
        TH(tx) "Expected a value" _TH
    }

    tx = values_read(&v2, tx, false);

    EACH(vs1, Value, v1) {
      arr_add(conditions, v1);
      arr_add(values, v2);
    }_EACH
  }

  *v = value_new_with(txpos_pos(start), value, conditions, values);
  return tx;
}

static Txpos *read_new(Value **v, Txpos *tx) {
  Txpos *start = tx;
  Txpos *r;
  char *id;
  if (txpos_eq(tx, r = token_id(&id, tx))) {
    return tx;
  }
  if (strcmp(id, "new")) {
    return tx;
  }
  tx = r;

  Type *tp;
  r = block_type(&tp, tx);

  char *tid = str_printf(" %s ", type_id(tp));
  switch (type_type(tp)) {
    case FN: TH(tx) "A function can not be declared 'new'" _TH
    case ANY: TH(tx) "A type 'Any' can not be declared 'new'" _TH
    case DATA:
      if (str_index(" Bool Byte Int Float Char Str ", tid) != -1)
        TH(tx) "A type '%s' can not be declared 'new'", type_id(tp) _TH
      break;
    default: THROW "Bad type" _THROW
  }
  tx = r;

  if (txpos_eq(tx, r = token_cconst(tx, '(')))
    TH(tx) "Expected '('" _TH
  tx = r;

  Arr/*Value*/ *vs;
  tx = token_list(
    &vs, tx, ')', (Txpos *(*)(void **, Txpos *))values_read0
  );

  *v = value_new_new(txpos_pos(start), tp, vs);
  return tx;
}

static Txpos *read_cast(Value **v, Txpos *tx) {
  Txpos *r;
  if (txpos_eq(tx, r = token_const(tx, "(:"))){
    return tx;
  }
  tx = r;

  Type *tp;
  if (txpos_eq(tx, r = block_type(&tp, tx)))
    TH(tx) "Expected a type" _TH
  tx = r;

  if (txpos_eq(tx, r = token_cconst(tx, ')')))
    TH(tx) "Expected ')'" _TH
  tx = r;

  tx = values_read(v, tx, false);
  value_set_type(*v, tp);

  return tx;
}

static Txpos *read_group(Value **v, Txpos *tx) {
  Txpos *start = tx;
  Txpos *r;
  if (txpos_eq(tx, r = token_cconst(tx, '('))) {
    return tx;
  }
  tx = r;

  Value *value;
  tx = values_read(&value, tx, true);

  Arr /*Attach*/ *atts = arr_new();
  tx = attachs_read_all(atts, tx);

  *v = value_new_group(txpos_pos(start), atts, value);
  return tx;
}

static Txpos *read_lunary(Value **v, Txpos *tx) {
  Txpos *start = tx;
  Txpos *r;
  char *op;
  if (txpos_eq(tx, r = token_split(&op, tx, "! ++ -- + -"))) {
    return tx;
  }
  tx = r;

  Value *value;
  tx = values_read(&value, tx, false);
  if (
    ((*op == '-' && *(op + 1) == '-') || (*op == '+' && *(op + 1) == '+'))
    &&
    (value_vtype(value) != VID || arr_size(value_attachs(value)))
  )
    TH(tx) "'%s' Only can be applied to an identifier", op _TH

  *v = value_new_lunary(txpos_pos(start), op, value);
  return tx;
}

static Txpos *read_runary(Value **v, Txpos *tx) {
  Txpos *start = tx;
  Txpos *r;
  char *op;
  if (txpos_eq(tx, r = token_split(&op, tx, "++ --"))) {
    return tx;
  }
  tx = r;

  if (value_vtype(*v) != VID || arr_size(value_attachs(*v)))
    TH(tx) "'%s' Only can be applied to an identifier", op _TH

  *v = value_new_runary(txpos_pos(start), op, *v);
  return tx;
}

static Txpos *read_binary(Value **v, Txpos *tx) {
  Txpos *start = tx;
  Txpos *r;
  char *op;
  if (txpos_eq(tx, r = token_split(&op, tx,
    "== != <= >= && || + - * / % ?: >>> ^^ << >> ^ > < & |"
  ))) {
    return tx;
  }

  Value *v1 = *v;
  Value *v2;
  tx = values_read(&v2, r, false);
  *v = value_new_binary(txpos_pos(start), op, v1, v2);
  return tx;
}

static Txpos *read_ternary(Value **v, Txpos *tx) {
  Txpos *start = tx;
  Txpos *r;
  if (txpos_eq(tx, r = token_cconst(tx, '?'))) {
    return tx;
  }

  Value *v1 = *v;
  Value *v2;
  tx = values_read(&v2, r, false);

  if (txpos_eq(tx, r = token_cconst(tx, ':')))
    TH(tx) "Expected ':' (colon)" _TH

  Value *v3;
  tx = values_read(&v3, r, false);

  *v = value_new_ternary(txpos_pos(start), v1, v2, v3);
  return tx;
}

static Txpos *read_fn(Value **v, Txpos *tx) {
  Txpos *start = tx;
  Txpos *r;
  Arr/*char*/ *params = arr_new();
  if (txpos_eq(
    tx,
    r = token_fn_list(&params, tx, (Txpos *(*)(void **, Txpos *))token_id)
  )) {
    return tx;
  }
  tx = r;

  EACH(params, char, p) {
    if (token_is_reserved(p))
      TH(start) "'%s' is a reserved word", p _TH
  }_EACH

  if (txpos_eq(tx, r = token_cconst(tx, '{')))
    TH(tx) "Expected '{'" _TH
  tx = r;

  Arr/*Stat*/ *stats;
  tx = stats_block(&stats, tx);

  *v = value_new_fn(txpos_pos(start), params, stats);
  return tx;
}

Txpos *values_read(Value **val, Txpos *tx, bool is_group) {
  Txpos *r;
  Value *vr;

  if (txpos_neq(tx, r = read_fn(&vr, tx))) {
   *val = vr;
    return r;
  } else if (txpos_neq(tx, r = read_lunary(&vr, tx))) {
    tx = r;
  } else if (txpos_neq(tx, r = read_cast(&vr, tx))) {
    tx = r;
  } else if (txpos_neq(tx, r = read_group(&vr, tx))) {
    tx = r;
  } else if (txpos_neq(tx, r = read_null(&vr, tx))) {
    tx = r;
  } else if(txpos_neq(tx, r = read_bool(&vr, tx))) {
    tx = r;
  } else if(txpos_neq(tx, r = read_float(&vr, tx))) {
    tx = r;
  } else if(txpos_neq(tx, r = read_byte(&vr, tx))) {
    tx = r;
  } else if(txpos_neq(tx, r = read_int(&vr, tx))) {
    tx = r;
  } else if(txpos_neq(tx, r = read_string(&vr, tx))) {
    tx = r;
  } else if(txpos_neq(tx, r = read_arr(&vr, tx))) {
    tx = r;
  } else if(txpos_neq(tx, r = read_map(&vr, tx))) {
    tx = r;
  } else if (txpos_neq(tx, r = read_with(&vr, tx))) {
    tx = r;
  } else if (txpos_neq(tx, r = read_new(&vr, tx))) {
    tx = r;
  } else if(txpos_neq(tx, r = read_id(&vr, tx))) {
    tx = r;
  } else {
    TH(tx) "Expected a value" _TH
  }

  if (txpos_neq(tx, r = read_runary(&vr, tx))) {
    tx = r;
  }

  if (txpos_neq(tx, r = read_binary(&vr, tx))) {
    tx = r;
  }

  if (txpos_neq(tx, r = read_ternary(&vr, tx))) {
    tx = r;
  }

  if (is_group) {
    if (txpos_eq(tx, r = token_cconst(tx, ')')))
      TH(tx) "Expected ')'" _TH
    tx = r;
  }

  *val = vr;
  return tx;
}

inline
Txpos *values_read0(Value **val, Txpos *tx) {
  return values_read(val, tx, false);
}

static Txpos *read_new_cvalue(Cvalue **cvalue, Txpos *tx, bool is_public) {
  Txpos *r;
  Txpos *start = tx;
  Type *tp;
  if (txpos_neq(tx, r = token_cconst(tx, ':'))) {
    tx = r;
    tx = block_type(&tp, tx);
  } else {
    tp = type_new_unknown();
  }

  char *id;
  if (txpos_eq(tx, r = token_id(&id, tx)))
    TH(tx) "Expected an identifier" _TH
  tx = r;

  if (!strcmp(id, "val")) {
    Dvalue *dvalue;
    tx = block_declaration(&dvalue, tx, tp);

    if (value_vtype(dvalue_value(dvalue)) == VNULL)
        TH(tx) "'val %s' can not be null", dvalue_id(dvalue) _TH

    *cvalue = cvalue_new_val(txpos_pos(start), is_public, false, dvalue);
  } else if (!strcmp(id, "var")) {
    Dvalue *dvalue;
    tx = block_declaration(&dvalue, tx, tp);

    *cvalue = cvalue_new_var(txpos_pos(start), is_public, false, dvalue);
  } else
    TH(tx) "Expected val or var" _TH

  return tx;
}

static Txpos *read_param (Kv/*Kv[Pos]*/ **param, Txpos *tx) {
  Txpos *r;
  Pos *pos = txpos_pos(tx);

  char *value = "";
  if (txpos_neq(tx, r = token_cconst0(tx, '+'))) {
    tx = r;
    value = "+";
  } else if (txpos_neq(tx, r = token_cconst0(tx, '-'))) {
    tx = r;
    value = "-";
  }

  char *id;
  r = token_id(&id, tx);
  if (token_is_reserved(id))
    TH(tx) "'%s' is a reserved word", id _TH
  tx = r;

  *param = kv_new(id, kv_new(value, pos));
  return tx;
}

Txpos *values_new_read(
  Arr/*Cvalue*/ **values, Txpos *tx, Dvalue *dvalue, bool is_public
) {
  Arr/*Cvalues*/ *a = arr_new();
  *values = a;

  Txpos *r;
  Txpos *start = tx;
  Arr/*Kv[Kv[Pos]]*/ *c_params = arr_new();
  if (txpos_eq(
    tx,
    r = token_fn_list(&c_params, tx, (Txpos *(*)(void **, Txpos *))read_param)
  )) {
    return tx;
  }
  tx = r;

  if (type_type(dvalue_type(dvalue)) != FN)
    THROW "dvalue ought to be a function" _THROW

  Arr/*Type*/ *dvalue_types = type_params(dvalue_type(dvalue));
  if (arr_size(c_params) != arr_size(dvalue_types) - 1)
    TH(start) "new parameters must be %d", arr_size(dvalue_types) - 1 _TH

  Arr/*char*/ *params = arr_new();
  RANGE0(i, arr_size(c_params)) {

    Kv *kv = arr_get(c_params, i);
    char *id = kv->key;
    char ptype = *(((Kv *)kv->value)->key);
    arr_add(params, id);

    if (ptype != '-') {
      Type *dvalue_type = arr_get(dvalue_types, i);
      Pos *pos = ((Kv *)kv->value)->value;

      Value *v = value_new_id(pos, arr_new(), id);

      Dvalue *dv = dvalue_new(pos, id);
      dvalue_set_tpos(dv, pos);
      dvalue_set_type(dv, dvalue_type);
      dvalue_set_value(dv, v);

      if (ptype) {
        arr_add(a, cvalue_new_val(pos, true, false, dv));
      } else {
        arr_add(a, cvalue_new_var(pos, true, false, dv));
      }
    }
  }_RANGE

  if (txpos_eq(tx, r = token_cconst(tx, '{')))
    TH(tx) "Expected '{'" _TH
  tx = r;

  if (txpos_neq(tx, r = token_directive(tx, "_public"))) {
    tx = r;

    if (txpos_eq(tx, r = token_cconst(tx, '{')))
      TH(tx) "Expected '{'" _TH
    tx = r;

    for (;;) {
      if (txpos_neq(tx, r = token_cconst(tx, ';'))) {
        tx = r;
        continue;
      }
      if (txpos_neq(tx, r = token_cconst(tx, '}'))) {
        tx = r;
        break;
      }

      Cvalue *cvalue;
      tx = read_new_cvalue(&cvalue, tx, true);
      arr_add(a, cvalue);
    }
  }

  if (txpos_neq(tx, r = token_directive(tx, "_private"))) {
    tx = r;

    if (txpos_eq(tx, r = token_cconst(tx, '{')))
      TH(tx) "Expected '{'" _TH
    tx = r;

    for (;;) {
      if (txpos_neq(tx, r = token_cconst(tx, ';'))) {
        tx = r;
        continue;
      }
      if (txpos_neq(tx, r = token_cconst(tx, '}'))) {
        tx = r;
        break;
      }

      Cvalue *cvalue;
      tx = read_new_cvalue(&cvalue, tx, false);
      arr_add(a, cvalue);
    }
  }

  start = tx;
  Arr/*Stat*/ *stats;
  tx = stats_block(&stats, tx);

  dvalue_set_value(dvalue, value_new_fn(txpos_pos(start), params, stats));

  arr_add(a, cvalue_new_val(txpos_pos(start), is_public, true, dvalue));

  return tx;
}
