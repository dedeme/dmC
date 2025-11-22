// Copyright 12-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "data/type.h"
#include "kut/js.h"
#include "char.h"
#include "cts.h"
#include "data/wrCtx.h"

Type *type_new (char *id, Arr *subtypes) {
  Type *this = MALLOC(Type);
  this->id = id;
  this->subtypes = subtypes;
  return this;
}

static Type tbool = {"b", NULL};
Type *type_bool (void) {
  if (!tbool.subtypes) tbool.subtypes = arr_new();
  return &tbool;
}

static Type tint = {"i", NULL};
Type *type_int (void) {
  if (!tint.subtypes) tint.subtypes = arr_new();
  return &tint;
}

static Type tfloat = {"f", NULL};
Type *type_float (void) {
  if (!tfloat.subtypes) tfloat.subtypes = arr_new();
  return &tfloat;
}

static Type tstring = {"s", NULL};
Type *type_string (void) {
  if (!tstring.subtypes) tstring.subtypes = arr_new();
  return &tstring;
}

char *type_oid (Type *this) {
  if (*this->id != '<')
    EXC_KUTT(str_f("Type '%s' is not an object", type_to_str(this)));
  return str_sub(this->id, 1, -1);
}

char *type_dmd (Type *this) {
  if (*this->id != '(')
    EXC_KUTT(str_f("Type '%s' is not a definition", type_to_str(this)));
  char *tx = str_sub(this->id, 1, -1);
  int ix = str_cindex(tx, '.');
  return ix == -1 ? "" : str_left(tx, ix);
}

char *type_did (Type *this) {
  if (*this->id != '(')
    EXC_KUTT(str_f("Type '%s' is not a definition", type_to_str(this)));
  char *tx = str_sub(this->id, 1, -1);
  return str_right(tx, str_cindex(tx, '.') + 1);
}

Type *type_atype (Type *this) {
  if (*this->id != 'a')
    EXC_KUTT(str_f("Type '%s' is not an array", type_to_str(this)));
  return *arr_begin(this->subtypes);
}

Type *type_rtype (Type *this) {
  if (*this->id != 'r')
    EXC_KUTT(str_f("Type '%s' is not an iterator", type_to_str(this)));
  return *arr_begin(this->subtypes);
}

Type *type_dtype (Type *this) {
  if (*this->id != 'd')
    EXC_KUTT(str_f("Type '%s' is not a dictionary", type_to_str(this)));
  return *arr_begin(this->subtypes);
}

Type *type_ttype (Type *this, int i) {
  if (*this->id != 't')
    EXC_KUTT(str_f("Type '%s' is not a tuple", type_to_str(this)));
  return arr_begin(this->subtypes)[i];
}

int type_fnpars (Type *this) {
  switch (*this->id) {
    case '0': return arr_size(this->subtypes);
    case '1': return arr_size(this->subtypes) - 1;
    default: return -1;
  }
}

Type *type_fptype (Type *this, int i) {
  char t = *this->id;
  if (t == '1' || t == '0') return arr_begin(this->subtypes)[i];
  EXC_KUTT(str_f("Type '%s' is not a function", type_to_str(this)));
  return NULL; // Unreachable
}

Opt *type_frtype (Type *this) {
  char t = *this->id;
  if (t == '0') return opt_none();
  if (t == '1') return opt_some(arr_peek(this->subtypes));
  EXC_KUTT(str_f("Type '%s' is not a function", type_to_str(this)));
  return NULL; // Unreachable
}

char *type_fctype (Type *this) {
  // Arr<char>
  Arr *apars = arr_new();
  for (int i = 0; i < type_fnpars(this); ++i) arr_push(apars, "Val");
  char *pars = arr_cjoin(apars, ',');

  char t = *this->id;
  if (t == '0') return str_f("void (*)(%s)", pars);
  if (t == '1') return str_f("Val (*)(%s)", pars);
  EXC_KUTT(str_f("Type '%s' is not a function", type_to_str(this)));
  return NULL; // Unreachable
}

int type_is_generic (Type *this) {
  char t = *this->id;
  return t >= 'A' && t <= 'Z';
}

// Rs<Type>
static Rs *read (char **rest, char *etx, char *text);

// Rs<Type>
static Rs *rerr (char *tx, char *msg) {
  return rs_fail(str_f("%s in '%s'", msg, tx));
}

// Rs<Type>
static Rs *read_object (char **rest, char *etx, char *tx) {
  int ix = str_cindex(tx, '>');
  if (ix == -1) return rerr (etx, "'>' is missing");
  char *tp = str_trim(str_left(tx, ix));
  if (!char_is_id(tp))
    return rerr(etx, str_f("'%s' is not a valid identifier", tp));
  *rest = tx + ix + 1;
  return rs_ok(type_new(str_f("<%s>", tp), arr_new()));
}

// Rs<Type>
static Rs *read_definition (char **rest, char *etx, char *tx) {
  int bix = str_cindex(tx, '|');
  int pix = str_cindex(tx, ')');

  if (bix == -1 || bix > pix) {
    if (pix == -1) return rerr(etx, "')' is missing");
    Rs *rs = char_mk_dot_id(str_trim(str_left(tx, pix)));
    char *id = rs_get(rs);
    if (!id) return rerr(etx, rs_error(rs));
    *rest = tx + pix + 1;
    return rs_ok(type_new(str_f("(%s)", id), arr_new()));
  }

  Rs *rs = char_mk_dot_id(str_trim(str_left(tx, bix)));
  char *id = rs_get(rs);
  if (!id) return rerr(etx, rs_error(rs));
  // Arr<Type>
  Arr *pars = arr_new();
  char *rst = str_ltrim(tx + bix + 1);
  for (;;) {
    if (*rst == ')') break;
    char *rst2 = NULL;
    Rs *rs = read(&rst2, etx, rst);
    Type *t = rs_get(rs);
    if (!t) return rs;
    arr_push(pars, t);
    rst = str_ltrim(rst2);
  }
  if (!arr_size(pars)) return rerr(etx, "Definition parameters are missing");

  *rest = rst + 1;
  return rs_ok(type_new(str_f("(%s)", id), pars));
}

// Rs<Type>
static Rs *read_arrays (char **rest, char *etx, char *tx) {
  char *t = str_trim(tx);
  if (!*t) return rerr(etx, "']' is missing");
  char ch = *t;
  if (ch == ']')
    return rerr(etx, "'[]' is not a valid type");

  switch (ch) {
    case '|': {
      t = str_ltrim(t + 1);
      if (!*t) return rerr(etx, "']' is missing");
      if (*t == ']') {
        *rest = t + 1;
        return rs_ok(type_new("0", arr_new()));
      }

      char *rst = NULL;
      Rs *rs = read(&rst, etx, t);
      Type *tp = rs_get(rs);
      if (!tp) return rs;
      rst = str_ltrim(rst);
      if (!*rst) return rerr(etx, "']' is missing");
      if (*rst != ']')
        return rerr(etx, str_f(
          "Expected ']', but fund '%s'", rst
        ));
      *rest = rst + 1;
      return rs_ok(type_new("1", arr_new_from(tp, NULL)));
    }
    case '!': {
      t = str_ltrim(t + 1);
      char *rst = NULL;
      Rs *rs = read(&rst, etx, t);
      Type *tp = rs_get(rs);
      if (!tp) return rs;
      rst = str_ltrim(rst);
      if (!*rst) return rerr(etx, "']' is missing");
      if (*rst != ']')
        return rerr(etx, str_f(
          "Expected ']', but fund '%s'", rst
        ));
      *rest = rst + 1;
      return rs_ok(type_new("r", arr_new_from(tp, NULL)));
    }
    case '.': {
      // Arr<Type>
      Arr *tps = arr_new();
      t = str_ltrim(t + 1);
      char *rst = NULL;
      for(;;) {
        Rs *rs = read(&rst, etx, t);
        Type *tp = rs_get(rs);
        if (!tp) return rs;
        arr_push(tps, tp);
        t = str_ltrim(rst);
        if (!*t) return rerr(etx, "']' is missing");
        if (*t == ']') {
          rst = t + 1;
          break;
        }
      }
      *rest = rst;
      return rs_ok(type_new("t", tps));
    }
    default: {
      int is_fn = FALSE;
      // Arr<Type>
      Arr *tps = arr_new();
      char *rst = NULL;
      for (;;) {
        Rs *rs = read(&rst, etx, t);
        Type *tp = rs_get(rs);
        if (!tp) return rs;
        arr_push(tps, tp);
        t = str_ltrim(rst);
        if (!*t) return rerr(etx, "']' is missing");
        if (*t == ']') {
          rst = t + 1;
          break;
        }
        if (is_fn) return rerr(etx, "Bad function return");
        if (*t == '|') {
          is_fn = TRUE;
          t = str_ltrim(t + 1);
          if (!*t) return rerr(etx, "']' is missing");
          if (*t == ']') {                   // Return if procedure
            *rest = t + 1;
            return rs_ok(type_new("0", tps));
          }
        }
      }
      *rest = rst;
      if (is_fn) return rs_ok(type_new("1", tps));
      if (arr_size(tps) > 1)
        return rerr(etx, "Array with more than one subtype");
      return rs_ok(type_new("a", tps));
    }
  }
}

// Rs<Type>
static Rs *read_dictionary (char **rest, char *etx, char *tx) {
  char *t = str_trim(tx);
  if (!*t) return rerr(etx, "'}' is missing");
  char ch = *t;
  if (ch == '}') return rerr(etx, "'{}' is not a valid type");
  char *rst = NULL;
  Rs *rs = read(&rst, etx, t);
  Type *tp = rs_get(rs);
  if (!tp) return rs;
  rst = str_ltrim(rst);
  if (!*rst) return rerr(etx, "'}' is missing");
  if (*rst != '}')
    return rerr(etx, str_f(
      "Expected '}', but fund '%s'", rst
    ));
  *rest = rst + 1;
  return rs_ok(type_new("d", arr_new_from(tp, NULL)));
}

// Rs<Type>
static Rs *read (char **rest, char *etx, char *text) {
    char *t = str_ltrim(text);
    if (!*t)
      return rerr(etx, "Unexpected end of type");

    *rest = t + 1;
    char ch = *t;
    switch (ch) {
      case 'b': return rs_ok(type_bool());
      case 'i': return rs_ok(type_int());
      case 'f': return rs_ok(type_float());
      case 's': return rs_ok(type_string());
      case '<': return read_object(rest, text, t + 1);
      case '(': return read_definition(rest, text, t + 1);
      case '[': return read_arrays(rest, text, t + 1);
      case '{': return read_dictionary(rest, text, t + 1);
      default:
        if (ch >= 'A' && ch <= 'Z')
          return rs_ok(type_new(str_new_c(ch), arr_new()));
        else
          return rerr(etx, str_f("Unexpected '%c'", ch));
    }
}

// Rs<Type>
Rs *type_read (char *tx) {
  char *rest = NULL;
  Rs *rs = read(&rest, tx, tx);
  Type *t = rs_get(rs);
  if (!t) return rs;

  if (*str_trim(rest))
    return rerr(tx, str_f("Extra charcters '%s'", rest));

  return rs_ok(t);
}

/// Returns the canonical representation of 'this'.
char *type_to_str (Type *this) {
  // Arr<Type>
  Arr *sbs = this->subtypes;
  switch (*this->id) {
    case 'a': return arr_size(sbs)
        ? str_f("[%s]", type_to_str(*arr_begin(sbs)))
        : "[]";
    case 'r': return str_f("[!%s]", type_to_str(*arr_begin(sbs)));
    case 'd': return arr_size(sbs)
        ? str_f("{%s}", type_to_str(*arr_begin(sbs)))
        : "{}";
    case '(': return arr_size(sbs)
        ? str_f("%s|%s)",
            str_left(this->id, -1),
            arr_join(arr_map(sbs, (FMAP)type_to_str), "")
          )
        : this->id;
    case 't': return str_f("[.%s]",
          arr_join(arr_map(sbs, (FMAP)type_to_str), "")
        );
    case '0': return str_f("[%s|]",
          arr_join(arr_map(sbs, (FMAP)type_to_str), "")
        );
    case '1': return arr_size(sbs)
        ? str_f("[%s|%s]",
            arr_join(arr_map(
              arr_take(sbs, arr_size(sbs) - 1),
              (FMAP)type_to_str), ""),
            type_to_str(arr_peek(sbs))
          )
        : "[?|?]";
    default: return this->id;
  }
}

char *type_to_c (Type *this, WrCtx *ctx) {
  Type *t2 = rs_get(wrCtx_expand(ctx, this));
  assert(t2);

  char *to_c (Type *t) {
    char *start = str_f(
      "__arr_new_from_carr((Val)2, (Val[]){(Val)\"%s\", ", t->id
    );
    char *rest = NULL;
    switch(*t->id) {
      case 'a':
      case 'r':
      case 'd':
        assert (arr_size(t->subtypes));
        rest = str_f(
          "__arr_new_from_carr((Val)1, (Val[]){%s})",
          to_c(*arr_begin(t->subtypes))
        );
        break;
      case 't':
        rest = str_f("__arr_new_from_carr((Val)%d, (Val[]){%s})",
          arr_size(t->subtypes),
          arr_cjoin(arr_map(t->subtypes, (FMAP)to_c), ',')
        );
        break;
      default:
        rest = "__arr_new()";
    }
    return str_f("%s%s})", start, rest);
  }
  return to_c(t2);
}

char *type_to_js (Type *this) {
  return js_wa(arr_new_from(
    js_ws(this->id),
    arr_to_js(this->subtypes, (FTO)type_to_js),
    NULL
  ));
}

Type *type_from_js (char *js) {
  char **a = (char **)arr_begin(js_ra(js));
  return type_new(
    js_rs(a[0]),
    arr_from_js(a[1], (FFROM)type_from_js)
  );
}

char *type_get_generics (Type *this) {
  char *get (char *gs, Type *t) {
    char ch = *t->id;
    switch (ch) {
      case 'a':
      case 'r':
      case 'd':
      case '(':
      case 't':
      case '0':
      case '1': {
        char *freduce (char *gens, Type *t) { return get(gens, t); }
        return arr_reduce(t->subtypes, gs, (void *(*)(void *,void *))freduce);
      }
      default: return ch >= 'A' && ch <= 'Z' && str_cindex(gs, ch) == -1
        ? str_f("%s%c", gs, ch)
        : gs
      ;
    }
  }
  return get("", this);
}

Type *type_replace_generics (Type *this, Map *gs) {
  char *id = this->id;
  char ch = *id;
  switch (ch) {
    case 'b':
    case 'i':
    case 'f':
    case 's':
    case '<': return this;
    default: {
      if (ch >= 'A' && ch <= 'Z') {
        Type *t = opt_get(map_get(gs, id));
        return (t) ? t : this;
      }
      Type *replace (Type *sbt) { return type_replace_generics(sbt, gs); }
      return type_new(
        id,
        arr_map(this->subtypes, (FMAP)replace)
      );
    }
  }
}

Type *type_replace_def_generics (
  Type *real_left, Type *left_def, Type *right_def
) {
  // Arr<Type>
  Arr *real_gs = real_left->subtypes;
  // Arr<Type>
  Arr *left_gs = left_def->subtypes;
  // Map<Type>
  Map *gs = map_new();
  EACH(left_gs, Type, g) {
    map_put(gs, g->id, arr_get(real_gs, _i));
  }_EACH
  return type_replace_generics(right_def, gs);
}

Rs *type_eq (WrCtx *ctx, Type *tp1, Type *tp2) {

  // pool is Arr<char>
  Rs *eq (Arr *pool, Type *tp1, Type *tp2) {
    char *id1 = tp1->id;
    char *id2 = tp2->id;

    char ch = *id1;
    switch (ch) {
      case 'b':
      case 'i':
      case 'f':
      case 's':
      case '<':
        return rs_ok(str_eq(id2, id1) ? cts_true() : cts_false());
      case 'a':
      case 'd':
        if (strcmp(id2, id1)) return rs_ok(cts_false());
        break;
        // else subtypes control
      default: {
        if (ch >= 'A' && ch <= 'Z')
          rs_ok(str_eq(id2, id1) ? cts_true() : cts_false());

        if (*id2 == '('  && ch == '(') {
          Rs *rs1 = wrCtx_canonical_def(ctx, type_dmd(tp1), type_did(tp1));
          char *can1 = rs_get(rs1);
          if (!can1) return rs_fail(rs_error(rs1));
          Rs *rs2 = wrCtx_canonical_def(ctx, type_dmd(tp2), type_did(tp2));
          char *can2 = rs_get(rs2);
          if (!can2) return rs_fail(rs_error(rs2));
          if (strcmp(can1, can2)) {
            int fany1 (char *id) { return str_eq(can1, id); }
            int in1 = arr_any(pool, (FPRED)fany1);
            int fany2 (char *id) { return str_eq(can2, id); }
            int in2 = arr_any(pool, (FPRED)fany2);
            if (!in1 || !in2) {
              if (!in1) arr_push(pool, can1);
              if (!in2) arr_push(pool, can2);
              Rs *rs1 = wrCtx_expand(ctx, tp1);
              Type *expand1 = rs_get(rs1);
              if (!expand1) return rs_fail(rs_error(rs1));
              Rs *rs2 = wrCtx_expand(ctx, tp2);
              Type *expand2 = rs_get(rs2);
              if (!expand2) return rs_fail(rs_error(rs2));
              return eq(pool, expand1, expand2);
            }
            // else subtypes control
          }
          // else subtypes control
        } else {
          if (ch == '(' ) {
            Rs *rs = wrCtx_expand(ctx, tp1);
            Type *expand = rs_get(rs);
            if (!expand) return rs_fail(rs_error(rs));
            return eq(pool, expand, tp2);
          }
          if (*id2 == '(' ) {
            Rs *rs = wrCtx_expand(ctx, tp2);
            Type *expand = rs_get(rs);
            if (!expand) return rs_fail(rs_error(rs));
            return eq(pool, tp1, expand);
          }
          if (strcmp(id1, id2)) return rs_ok(cts_false());
          // else subtypes control
        }
      }
    }
    // Subtypes control
    if (arr_size(tp1->subtypes) != arr_size(tp2->subtypes))
      return rs_ok(cts_false());
    Rs *r = rs_ok(cts_true());
    // Arr<Type>
    Arr *tp2_subtypes = tp2->subtypes;
    EACH(tp1->subtypes, Type, t1) {
      if (!rs_get(r) || !*((int *)rs_get(r))) break;
      Type *t2 = arr_begin(tp2_subtypes)[_i];
      r = eq(pool, t1, t2);
    }_EACH
    return r;
  }

  return eq(arr_new(), tp1, tp2);
}

Rs *type_allow (WrCtx *ctx, Map *generics, Type *expected, Type *actual) {
  // pool is Arr<char>
  Rs *allow (Arr *pool, Type *expected, Type *actual) {
    char *id1 = expected->id;
    char *id2 = actual->id;
    char ch = *id1;
    switch (ch) {
      case 'b':
      case 'i':
      case 'f':
      case 's':
      case '<':
        return rs_ok(str_eq(id2, id1) ? cts_true() : cts_false());
      case 'a':
      case 'd':
        if (strcmp(id2, id1)) return rs_ok(cts_false());
        break;
        // else subtypes control
      default: {
        if (ch >= 'A' && ch <= 'Z') {
          Type *t = opt_get(map_get(generics, id1));
          if (!t) {
            map_put(generics, id1, actual);
            return rs_ok(cts_true());
          }
          if (*t->id >= 'A' && *t->id <= 'Z')
            return rs_ok(str_eq(t->id, id2) ? cts_true() : cts_false());
          return type_allow(ctx, map_new(), t, actual);
        }

        if (*id2 == '('  && ch == '(') {
          Rs *rs1 = wrCtx_canonical_def(ctx, type_dmd(expected), type_did(expected));
          char *can1 = rs_get(rs1);
          if (!can1) return rs_fail(rs_error(rs1));
          Rs *rs2 = wrCtx_canonical_def(ctx, type_dmd(actual), type_did(actual));
          char *can2 = rs_get(rs2);
          if (!can2) return rs_fail(rs_error(rs2));

          if (strcmp(can1, can2)) {
            int fany1 (char *id) { return str_eq(can1, id); }
            int in1 = arr_any(pool, (FPRED)fany1);
            int fany2 (char *id) { return str_eq(can2, id); }
            int in2 = arr_any(pool, (FPRED)fany2);
            if (!in1 || !in2) {
              if (!in1) arr_push(pool, can1);
              if (!in2) arr_push(pool, can2);
              Rs *rs1 = wrCtx_expand(ctx, expected);
              Type *expand1 = rs_get(rs1);
              if (!expand1) return rs_fail(rs_error(rs1));
              Rs *rs2 = wrCtx_expand(ctx, actual);
              Type *expand2 = rs_get(rs2);
              if (!expand2) return rs_fail(rs_error(rs2));

              return allow(pool, expand1, expand2);
            }
            // else subtypes control
          }
          // else subtypes control
        } else {
          if (ch == '(' ) {
            Rs *rs = wrCtx_expand(ctx, expected);
            Type *expand = rs_get(rs);
            if (!expand) return rs_fail(rs_error(rs));
            return allow(pool, expand, actual);
          }
          if (*id2 == '(' ) {
            Rs *rs = wrCtx_expand(ctx, actual);
            Type *expand = rs_get(rs);
            if (!expand) return rs_fail(rs_error(rs));
            return allow(pool, expected, expand);
          }
          if (strcmp(id1, id2)) return rs_ok(cts_false());
          // else subtypes control
        }
      }
    }

    // Subtypes control
    // Arr<Type>
    Arr *egs = expected->subtypes;
    // Arr<Type>
    Arr *ags = actual->subtypes;
    if (arr_size(egs) > arr_size(ags)) return rs_ok(cts_false());

    Rs *r = rs_ok(cts_true());
    EACH(egs, Type, t1) {
      if (!rs_get(r) || !*((int *)rs_get(r))) break;
      Type *t2 = arr_begin(ags)[_i];
      r = allow(pool, t1, t2);
    }_EACH
    return r;
  }

  return allow(arr_new(), expected, actual);
}

Rs *type_allow_return (WrCtx *ctx, Type *expected, Type *actual) {

  // pool is Arr<char>
  Rs *allow_return (Arr *pool, Type *expected, Type *actual) {
    char *id1 = expected->id;
    char *id2 = actual->id;

    char ch = *id1;
    switch (ch) {
      case 'b':
      case 'i':
      case 'f':
      case 's':
      case '<':
        return rs_ok(str_eq(id2, id1) ? cts_true() : cts_false());
      case 'a':
      case 'd':
        if (strcmp(id2, id1)) return rs_ok(cts_false());
        break;
        // else subtypes control
      default: {
        if (ch >= 'A' && ch <= 'Z') return rs_ok(cts_true());

        if (*id2 == '('  && ch == '(') {
          Rs *rs1 = wrCtx_canonical_def(ctx, type_dmd(expected), type_did(expected));
          char *can1 = rs_get(rs1);
          if (!can1) return rs_fail(rs_error(rs1));
          Rs *rs2 = wrCtx_canonical_def(ctx, type_dmd(actual), type_did(actual));
          char *can2 = rs_get(rs2);
          if (!can2) return rs_fail(rs_error(rs2));

          if (strcmp(can1, can2)) {
            int fany1 (char *id) { return str_eq(can1, id); }
            int in1 = arr_any(pool, (FPRED)fany1);
            int fany2 (char *id) { return str_eq(can2, id); }
            int in2 = arr_any(pool, (FPRED)fany2);
            if (!in1 || !in2) {
              if (!in1) arr_push(pool, can1);
              if (!in2) arr_push(pool, can2);
              Rs *rs1 = wrCtx_expand(ctx, expected);
              Type *expand1 = rs_get(rs1);
              if (!expand1) return rs_fail(rs_error(rs1));
              Rs *rs2 = wrCtx_expand(ctx, actual);
              Type *expand2 = rs_get(rs2);
              if (!expand2) return rs_fail(rs_error(rs2));
              return allow_return(pool, expand1, expand2);
            }
            // else subtypes control
          }
          // else subtypes control
        } else {
          if (ch == '(' ) {
            Rs *rs = wrCtx_expand(ctx, expected);
            Type *expand = rs_get(rs);
            if (!expand) return rs_fail(rs_error(rs));
            return allow_return(pool, expand, actual);
          }
          if (*id2 == '(' ) {
            Rs *rs = wrCtx_expand(ctx, actual);
            Type *expand = rs_get(rs);
            if (!expand) return rs_fail(rs_error(rs));
            return allow_return(pool, expected, expand);
          }
          if (strcmp(id1, id2)) return rs_ok(cts_false());
          // else subtypes control
        }
      }
    }
    // Subtypes control
    // Arr<Type>
    Arr *egs = expected->subtypes;
    // Arr<Type>
    Arr *ags = actual->subtypes;
    if (arr_size(egs) > arr_size(ags)) return rs_ok(cts_false());

    Rs *r = rs_ok(cts_true());
    EACH(egs, Type, t1) {
      if (!rs_get(r) || !*((int *)rs_get(r))) break;
      Type *t2 = arr_begin(ags)[_i];
      r = allow_return(pool, t1, t2);
    }_EACH
    return r;
  }

  return allow_return(arr_new(), expected, actual);
}

char *type_check_def (char *gs, Type *this) {
  char *add_gs (char *gens, Type *tp) {
    char tid = *tp->id;
    switch (tid) {
      case 'a':
      case 'r':
      case 'd':
      case '(':
      case 't':
      case '0':
      case '1': {
        EACH(tp->subtypes, Type, t) {
          gens = add_gs(gens, t);
        }_EACH
        return gens;
      }
      default: {
        if (tid >= 'A' && tid <= 'Z' && str_cindex(gens, tid) == -1)
          gens = str_f("%s%c", gens, tid);
        return gens;
      }
    }
  }

  char *old_gs = add_gs("", this);
  int gs_len = strlen(gs);
  if (gs_len != strlen(old_gs))
    return "Number of generics does not match";

  for (int i = 0; i < gs_len; ++i) {
    if (str_cindex(old_gs, gs[i]) == -1)
      return str_f(
        "Generic symbol '%c' of new type is not in the old one.", gs[i]
      );
  }

  return "";
}
