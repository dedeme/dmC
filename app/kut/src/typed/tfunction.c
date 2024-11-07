// Copyright 11-Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "kut/thread.h"
#include "kut/path.h"
#include "fileix.h"
#include "symix.h"
#include "obj.h"
#include "typed/tfunction.h"
#include "typed/treader.h"
#include "typed/tarr.h"
#include "typed/genc.h"

struct tfunction_Tfunction {
  char *key;
  char *id;
  char *cid;
  Iarr *tpars;
  Ttype tret;
  Iarr *pars;
  StatCode *st_cd;
};

Tfunction *tfunction_new (
  Iarr *tpars, Ttype tret, Iarr *pars, StatCode *stat
) {
  int fix = stat_code_file_ix(stat);
  char *f = str_new(fileix_to_fail(fix));
  char *p = f;
  while (*p) {
    char ch = *p;
    if (!(
      (ch >= '0' && ch <= '9') ||
      (ch >= 'A' && ch <= 'Z') ||
      (ch >= 'a' && ch <= 'z')
    ))
      *p = '_';
    ++p;
  }

  Tfunction *this = MALLOC(Tfunction);
  this->key = f;
  this->id = "";
  this->cid = "";
  this->tpars = tpars;
  this->tret = tret;
  this->pars = pars;
  this->st_cd = stat;
  return this;
}

void tfunction_set_id (Tfunction *this, char *id) {
  this->id = id;
  this->cid = str_f("%s_%s", str_left(this->key, -4), id);
}

char *tfunction_get_id (Tfunction *this) {
  return this->id;
}

/// Returns tfunction C identifier.
char *tfunction_get_cid (Tfunction *this) {
  return this->cid;
}

Iarr *tfunction_get_tpars (Tfunction *this) {
  return this->tpars;
}

Ttype tfunction_get_tret (Tfunction *this) {
  return this->tret;
}

Iarr *tfunction_get_pars (Tfunction *this) {
  return this->pars;
}

StatCode *tfunction_get_st_cd (Tfunction *this) {
  return this->st_cd;
}

// pars is Arr<Exp>. Expressions are solved.
static Exp *tfunction_run2 (Tfunction *this, Arr *pars) {
  CHECK_PARS("<tfunction>", iarr_size(this->pars), pars);

  Iarr *tpars = this->tpars;
  Ttype tret = this->tret;
  // <Tval>
  Arr *cpars = arr_new();
  EACH(pars, Exp, e) {
    Tval *v = MALLOC(Tval);
    if (obj_is_typed(e)) {
      if (obj_get_typed_type(e) != (Ttype)iarr_get(tpars, _i)) {
        EXC_KUT(str_f(
          "Expected object<typed> of type '%s' for parameter %d of %d, "
          "but '%s' was found",
          ttype_to_str(iarr_get(tpars, _i)), _i, arr_size(pars),
          obj_get_typed_type(e)
        ));
      }
      *v = obj_get_typed_value(e);
    } else {
      switch ((Ttype) iarr_get(tpars, _i)) {
        case ttype_BOOL:
          v->i = exp_get_bool(e);
          break;
        case ttype_INT:
          v->i = exp_get_int(e);
          break;
        case ttype_FLOAT:
          v->f = exp_get_float(e);
          break;
        case ttype_STRING:
          v->s = exp_get_string(e);
          break;
        case ttype_AINT:
          v->I = tarr_fromi(exp_get_array(e));
          break;
        case ttype_AFLOAT:
          v->F = tarr_fromf(exp_get_array(e));
          break;
        case ttype_ASTRING:
          v->S = tarr_froms(exp_get_array(e));
          break;
        case ttype_DIC: {
          v->S = tarr_fromd(exp_get_dic(e));
          break;
        }
        default: {
          v->i = 0; // To avoid warning.
          EXC_KUT(str_f(
            "Unexpected parameter of type '%s'",
            ttype_to_str((Ttype) iarr_get(tpars, _i))
          ));
        }
      }
    }
    arr_push(cpars, v);
  }_EACH

  int n = arr_size(cpars);
  Tval *vpars = GC_MALLOC(sizeof(Tval) * n);
  Tval *pvars = vpars;
  EACH(cpars, Tval, v) {
    *pvars++ = *v;
  }_EACH
  if (tret == ttype_NO_RETURN) {
    genc_run_procedure(this->cid, vpars);
    return exp_empty_return();
  } else {
    Tval ret = genc_run_function(this->cid, vpars);
    return obj_typed(tret, ret);
  }
}

Exp *tfunction_run (Tfunction *this, Arr *pars) {
  Exp *r = NULL;
  void fn (void) {
    r = tfunction_run2(this, pars);
  }
  thread_sync(fn);
  return r;
}

Exp *tfunction_untype(Ttype type, Tval value) {
  switch (type) {
    case ttype_BOOL:
      return exp_bool(value.i);
    case ttype_INT:
      return exp_int(value.i);
    case ttype_FLOAT:
      return exp_float(value.f);
    case ttype_STRING: {
      char *r = str_new(value.s);
      return exp_string(r);
    }
    case ttype_AINT:
      return exp_array(tarr_to_arri(value.I));
    case ttype_AFLOAT:
      return exp_array(tarr_to_arrf(value.F));
    case ttype_ASTRING:
      return exp_array(tarr_to_arrs(value.S));
    case ttype_DIC:
      return exp_dic(tarr_to_arrd(value.S));
    default: // do nothing
  }

  EXC_KUT(str_f("Typed value '%s' can not be untyped.", ttype_to_str));
  return exp_empty_return(); // Unreachable
}

char *tfunction_to_str (Tfunction *this) {
  return str_f(
    "Tfunction<%s|%s> %s",
    ttype_group_to_str(this->tpars),
    ttype_to_str(this->tret),
    this->id
  );
}

