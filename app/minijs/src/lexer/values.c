// Copyright 27-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/values.h"
#include "lexer/strings.h"
#include "lexer/numbers.h"
#include "lexer/token.h"
#include "lexer/stats.h"
#include "DEFS.h"

static Txpos *read_null(Value **v, Txpos *tx) {
  Txpos *r;
  if (txpos_neq(tx, r = token_const(tx, "null"))) {
    *v = value_new_null();
    tx = r;
  }
  return tx;
}

static Txpos *read_bool(Value **v, Txpos *tx) {
  Txpos *r;
  char *value;
  if (txpos_neq(tx, r = token_bool(&value, tx))) {
    *v = value_new_bool(value);
    tx = r;
  }
  return tx;
}

static Txpos *read_byte(Value **v, Txpos *tx) {
  Txpos *r;
  char *value;
  if (txpos_neq(tx, r = numbers_byte(&value, tx))) {
    *v = value_new_byte(value);
    tx = r;
  }
  return tx;
}

static Txpos *read_int(Value **v, Txpos *tx) {
  Txpos *r;
  char *value;
  if (txpos_neq(tx, r = numbers_int(&value, tx))) {
    *v = value_new_int(value);
    tx = r;
  }
  return tx;
}

static Txpos *read_float(Value **v, Txpos *tx) {
  Txpos *r;
  char *value;
  if (txpos_neq(tx, r = numbers_float(&value, tx))) {
    *v = value_new_float(value);
    tx = r;
  }
  return tx;
}

static Txpos *read_string(Value **v, Txpos *tx) {
  Txpos *r;
  char *value;
  enum Vtype_t type;
  if (txpos_neq(tx, r = strings_read(&type, &value, tx))) {
    switch (type) {
    case VSTR:
      *v = value_new_str(value);
      break;
    case VCHAR:
      *v = value_new_char(value);
      break;
    case VSTR2:
      *v = value_new_str2(value);
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
  if (txpos_neq(tx, r = token_point_id(&value, tx))) {
    *v = value_new_id(value);
    tx = r;
  }
  return tx;
}

static Txpos *read_fid(Value **v, Txpos *tx) {
  Txpos *start = tx;
  Txpos *r;
  char *id;
  if (txpos_eq(tx, r = token_point_id(&id, tx))) {
    return tx;
  }
  tx = r;

  if (txpos_eq(tx, r = token_cconst(tx, '('))) {
    return start;
  }
  tx = r;

  Arr/*Value*/ *list;
  Txpos *read(void **value, Txpos *tx) {
    return values_read((Value **)value, tx, false);
  }
  tx = token_list(&list, tx, ')', read);

  *v = value_new_fid(id, list);

  return tx;
}

static Txpos *read_arr(Value **v, Txpos *tx) {
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

  *v = value_new_arr(list);

  return tx;
}

static Txpos *read_map(Value **v, Txpos *tx) {
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
    } else if (txpos_neq(tx, r = read_id(&kv, tx))) {
      arr_add(key_value, kv);
      tx = r;
    } else
      TH(tx) "Map key must be a string" _TH

    if (txpos_eq(tx, r = token_cconst(tx, ':')))
      TH(tx) "Expected ':' (colon)" _TH
    tx = r;

    tx = values_read(&kv, tx, false);
    arr_add(key_value, kv);

    *value = (void **)key_value;
    return tx;
  }
  tx = token_list(&list, tx, '}', read);

  Arr/*Value*/ *keys = arr_new();
  Arr/*Value*/ *values = arr_new();
  RANGE0(i, arr_size(list)) {
    Arr/*Value*/ *kv = arr_get(list, i);
    arr_add(keys, arr_get(kv, 0));
    arr_add(values, arr_get(kv, 1));
  }_RANGE

  *v = value_new_map(keys, values);
  return tx;
}

static Txpos *read_with(Value **v, Txpos *tx) {
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
  Value *v1, *v2;
  bool cont = true;
  while (cont) {
    if (txpos_eq(tx, r = token_cconst(tx, ':')))
      TH(tx) "Expected ':' (colon)" _TH
    tx = r;

    if (txpos_neq(tx, r = token_cconst(tx, '_'))) {
      v1 = value_new_null();
      tx = r;
      cont = false;
    } else {
      tx = values_read(&v1, tx, false);
    }

    if (txpos_eq(tx, r = token_cconst(tx, '=')))
      TH(tx) "Expected '='" _TH
    tx = r;

    tx = values_read(&v2, tx, false);

    arr_add(conditions, v1);
    arr_add(values, v2);
  }

  *v = value_new_with(value, conditions, values);
  return tx;
}

static Txpos *read_group(Value **v, Txpos *tx) {
  Txpos *r;
  if (txpos_eq(tx, r = token_cconst(tx, '('))) {
    return tx;
  }
  tx = r;

  Value *value;
  tx = values_read(&value, tx, true);

  *v = value_new_group(value);
  return tx;
}

static Txpos *read_lunary(Value **v, Txpos *tx) {
  Txpos *r;
  char *op;
  if (txpos_eq(tx, r = token_split(&op, tx, "! ++ -- + -"))) {
    return tx;
  }
  tx = r;

  Value *value;
  tx = values_read(&value, tx, false);

  *v = value_new_lunary(op, value);
  return tx;
}

static Txpos *read_runary(Value **v, Txpos *tx) {
  Txpos *r;
  char *op;
  if (txpos_eq(tx, r = token_split(&op, tx, "++ --"))) {
    return tx;
  }
  tx = r;

  *v = value_new_runary(op, *v);
  return tx;
}

static Txpos *read_binary(Value **v, Txpos *tx) {
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
  *v = value_new_binary("", v1, v2);
  return tx;
}

static Txpos *read_ternary(Value **v, Txpos *tx) {
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

  *v = value_new_ternary(v1, v2, v3);
  return tx;
}

static Txpos *read_fn(Value **v, Txpos *tx) {
  Txpos *r;
  Arr/*char*/ *params = arr_new();
  if (txpos_eq(
    tx,
    r = token_fn_list(&params, tx, (Txpos *(*)(void **, Txpos *))token_id)
  )) {
    return tx;
  }
  tx = r;

  Arr/*Stat*/ *stats;
  tx = stats_block(&stats, tx);

  *v = value_new_fn(params, stats);
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
  } else if(txpos_neq(tx, r = read_fid(&vr, tx))) {
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
