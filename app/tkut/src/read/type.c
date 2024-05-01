// Copyright 04-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "kut/tp.h"
#include "read/type.h"
#include "kut/map.h"

static Type *new (TypeT type, void *value) {
  Type *this = MALLOC(Type);
  this->type = type;
  this->value = value;
  return this;
}

Type *type_mk_bool () {
  return new(TP_BOOL, NULL);
}

Type *type_mk_int () {
  return new(TP_INT, NULL);
}

Type *type_mk_float () {
  return new(TP_FLOAT, NULL);
}

Type *type_mk_string () {
  return new(TP_STRING, NULL);
}

Type *type_mk_generic (char *id) {
  return new(TP_GENERIC, id);
}

char *type_generic_id (Type *this) {
  return this->value;
}

Type *type_mk_object (char *id, Arr *generics) {
  return new(TP_OBJECT, tp_new(id, generics));
}

char *type_object_id (Type *this) {
  return tp_e1(this->value);
}

// <Type> (generic types)
Arr *type_object_generics (Type *this) {
  return tp_e2(this->value);
}

// types is Arr<Type>
Type *type_mk_tuple (Arr *types) {
  return new(TP_TUPLE, types);
}

// <Type>
Arr *type_tuple_types (Type *this) {
  return this->value;
}

Type *type_mk_arr (Type *type) {
  return new(TP_ARR, type);
}

Type *type_arr_type (Type *this) {
  return this->value;
}

// types is Arr<Type>
Type *type_mk_rec (Arr *types) {
  return new(TP_REC, types);
}

// <Type>
Arr *type_rec_types (Type *this) {
  return this->value;
}

Type *type_mk_dic (Type *type) {
  return new(TP_DIC, type);
}

Type *type_dic_type (Type *this) {
  return this->value;
}

// params is Arr<Type>
// ret is Opt<Type>
Type *type_mk_func (Arr *params, Opt *ret) {
  return new(TP_FUNC, tp_new(params, ret));
}

// <Type>
Arr *type_func_params (Type *this) {
  return tp_e1(this->value);
}

// <Type>
Opt *type_func_return (Type *this) {
  return tp_e2(this->value);
}

// -----------------------------------------------------------------------------

// gens is a Map<Type>
static int ok (Map *gens, Type *this, Type *rq) {
  TypeT rq_t = rq->type;
  switch (rq_t) {
    case TP_BOOL:
    case TP_INT:
    case TP_FLOAT:
    case TP_STRING:
      return rq_t == this->type;
    case TP_GENERIC: {
      char *r_id = type_generic_id(rq);
      Type *rq2 = opt_get(map_get(gens, r_id));
      if (rq2) {
        if (rq2->type != TP_GENERIC) return ok(gens, this, rq2);
        return this->type == TP_GENERIC &&
               !strcmp(type_generic_id(this), r_id);
      }
      map_put(gens, r_id, this);
      return TRUE;
    }
    case TP_OBJECT:
      if (rq_t == this->type) {
        if (str_eq(type_object_id(this), type_object_id(rq))) {
          // <Type>
          Arr *tgens = type_object_generics(this);
          // <Type>
          Arr *rgens = type_object_generics(rq);
          if (arr_size(tgens) == arr_size(rgens)) {
            EACH(rgens, Type, rg) {
              if (!ok(gens, arr_get(tgens, _i), rg)) return FALSE;
            }_EACH
            return TRUE;
          }
        }
      }
      return FALSE;
    case TP_TUPLE:
    case TP_REC:
      if (rq_t == this->type) {
        // <Type>
        Arr *tgens = this->value;
        // <Type>
        Arr *rgens = rq->value;
        if (arr_size(tgens) == arr_size(rgens)) {
          EACH(rgens, Type, rg) {
            if (!ok(gens, arr_get(tgens, _i), rg)) return FALSE;
          }_EACH
          return TRUE;
        }
      }
      return FALSE;
    case TP_ARR:
    case TP_DIC:
      return rq_t == this->type &&
        ok(gens, this->value, rq->value);
    case TP_FUNC:
      if (rq_t == this->type) {
          // <Type>
          Arr *tgens = type_func_params(this);
          // <Type>
          Arr *rgens = type_func_params(rq);
          if (arr_size(tgens) == arr_size(rgens)) {
            EACH(rgens, Type, rg) {
              if (!ok(gens, arr_get(tgens, _i), rg)) return FALSE;
            }_EACH
            Type *tret = opt_get(type_func_return(this));
            Type *rret = opt_get(type_func_return(rq));
            return rret
              ? tret && ok(gens, tret, rret)
              : !tret
            ;
          }
      }
      return FALSE;
  }
  EXC_GENERIC("Type of type not valid");
  return FALSE; // Unreachable
}

int type_ok (Type *this, Type *rq) {
  return ok(map_new(), this, rq);
}

static Type *from_str(char *s, int *ix) {
  int len = strlen(s);

  void skip_blanks () {
    int i = *ix;
    while (i < len) {
      if (s[i] > ' ') break;
      ++i;
    }
    if (i == len) EXC_KUT(str_f("Unexpected end of type definition in '%s'", s));
    *ix = i;
  }

  Type *from (int *ix) {
    char ch = s[(*ix)++];
    if (ch >= 'A' && ch <= 'Z') return type_mk_generic(str_new_c(ch));
    switch (ch) {
      case 'b':
        return type_mk_bool();
      case 'i':
        return type_mk_int();
      case 'f':
        return type_mk_float();
      case 's':
        return type_mk_string();
      case '[': {
        skip_blanks();
        ch = s[(*ix)++];
        if (ch == ']') return type_mk_tuple(arr_new());
        Type *t = from(ix);
        skip_blanks();
        ch = s[(*ix)++];
        if (ch == '.') {
          skip_blanks();
          ch = s[(*ix)++];
          if (ch != ']') EXC_KUT(str_f(
            "Expected character ']' in type definition '%s': %s?%s",
            s, str_left(s, *ix - 1), str_right(s, *ix - 1)
          ));
          return type_mk_arr(t);
        }
        // <Type>
        Arr *r = arr_new_from(t, NULL);
        if (ch == ']') return type_mk_tuple(r);
        --(*ix);
        for (;;) {
          arr_push(r, from(ix));
          skip_blanks();
          ch = s[(*ix)++];
          if (ch == ']') return type_mk_tuple(r);
          if (ch != ',') EXC_KUT(str_f(
            "Expected character ',' in type definition '%s': %s?%s",
            s, str_left(s, *ix - 1), str_right(s, *ix - 1)
          ));
          skip_blanks();
        }
      }
      case '{': {
        skip_blanks();
        ch = s[(*ix)++];
        if (ch == '}') return type_mk_rec(arr_new());
        Type *t = from(ix);
        skip_blanks();
        ch = s[(*ix)++];
        if (ch == '.') {
          skip_blanks();
          ch = s[(*ix)++];
          if (ch != '}') EXC_KUT(str_f(
            "Expected character '}' in type definition '%s': %s?%s",
            s, str_left(s, *ix - 1), str_right(s, *ix - 1)
          ));
          return type_mk_dic(t);
        }
        // <Type>
        Arr *r = arr_new_from(t, NULL);
        if (ch == '}') return type_mk_rec(r);
        --(*ix);
        for (;;) {
          arr_push(r, from(ix));
          skip_blanks();
          ch = s[(*ix)++];
          if (ch == '}') return type_mk_rec(r);
          if (ch != ',') EXC_KUT(str_f(
            "Expected character ',' in type definition '%s': %s?%s",
            s, str_left(s, *ix - 1), str_right(s, *ix - 1)
          ));
          skip_blanks();
        }
      }
      case '<': {
        skip_blanks();
        ch = s[(*ix)++];
        char *id = "";
        while (
          ch >= '0' && ch <= '9'
        ) {
          char *id = str_f("%s%c", id, ch);
          ch = s[(*ix)++];
        }
        if (!*id) EXC_KUT(str_f(
          "Expected character identifier in type definition '%s': %s?%s",
          s, str_left(s, *ix - 1), str_right(s, *ix - 1)
        ));
        skip_blanks();
        // <Type>
        Arr *a = arr_new();
        ch = s[(*ix)++];
        if (ch == '|') {
          for (;;) {
            skip_blanks();
            arr_push(a, from(ix));
            skip_blanks();
            ch = s[(*ix)++];
            if (ch != ',') break;
          }
        }
        if (ch != '>') EXC_KUT(str_f(
          "Expected character '>' in type definition '%s': %s?%s",
          s, str_left(s, *ix - 1), str_right(s, *ix - 1)
        ));
        return type_mk_object(id, a);
      }

    }
    EXC_KUT(str_f(
      "Unexpected character in type definition '%s': %s?%s",
      s, str_left(s, *ix - 1), str_right(s, *ix - 1)
    ));
    return NULL; // Unreachable
  }

  return from(ix);
}

Type *type_from_str (char *s) {
  s = str_trim(s);
  if (!*s) EXC_KUT("Type definition is an empty string.");
  int ix = 0;
  Type *t = from_str(s, &ix);
  char *rest = str_right(s, ix);
  if (*rest) EXC_KUT(str_f("Sparse characters in type definition (%s).", rest));
  return t;
}
