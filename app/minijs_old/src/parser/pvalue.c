// Copyright 16-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "parser/pvalue.h"
#include "ast/Vatt.h"
#include "ast/Program.h"
#include "ast/Avalue.h"
#include "lexer/Tx.h"
#include "DEFS.h"

static void type_check(
  Class *c, Pos *pos, Type *tp, Type *child, bool(*ok)(Type *, Type *)
) {
  if (!ok(tp, child))
    TH2(c, pos)
      "'%s' can not be assigned to '%s'", type_to_str(child), type_to_str(tp)
    _TH
}

static Type *tattach(Class *c, Pos *pos, Lsymbol *ss, Type *tp, Vatt *a) {
  if (vatt_is_fn(a)) {
    Atype *tps = type_params(tp);
    Avalue *vps = vatt_params(a);
    if (arr_size(tps) - 1 != arr_size(vps))
      TH2(c, pos)
        "Expected '%d' parameters, found '%d'", arr_size(tps) - 1, arr_size(vps)
      _TH
    EACH(vps, Value, v) {
      pvalue(c, ss, atype_get(tps, _i), v, type_child);
    }_EACH
    return atype_get(tps, arr_size(tps) - 1);
  }
  Class *tc = program__class(program_get(), type_id(tp));
  if (!tc)
    TH2(c, pos) "Class '%s' is unknown", type_id(tp) _TH

  char *id = vatt_id(a);
  Type *r = NULL;
  EACH((Arr *)class_instance_all(tc, type_params(tp)), Att, att) {
    if (!strcmp(att_id(att), id)) {
      tp = att_type(att);
    }
  }_EACH
  if (!r) {
    TH2(c, pos) "Attribute '%s' is missing in '%s'", id, type_id(tp) _TH
  }

  return r;
}

static Type *tattachs(Class *c, Pos *pos, Lsymbol *ss, Type *tp, Avatt *as) {
  EACH(as, Vatt, a) {
    tp = tattach(c, pos, ss, tp, a);
  }_EACH
  return tp;
}

static void pdata(
  Class *c, Lsymbol *ss, Type *tp, Value *v, bool(*ok)(Type *, Type *)
) {
  Pos *pos = value_pos(v);
  Avatt *attachs = value_attachs(v);
  switch(value_vtype(v)) {
    case VNULL:
      value_set_type(v, tp);
      break;
    case VBOOL:
      value_set_type(v, tattachs(c, pos, ss, type_new_bool(), attachs));
      type_check(c, pos, tp, value_type(v), ok);
      break;
    case VBYTE:
      value_set_type(v, tattachs(c, pos, ss, type_new_byte(), attachs));
      type_check(c, pos, tp, value_type(v), ok);
      break;
    case VINT:
      value_set_type(v, tattachs(c, pos, ss, type_new_int(), attachs));
      type_check(c, pos, tp, value_type(v), ok);
      break;
    case VFLOAT:
      value_set_type(v, tattachs(c, pos, ss, type_new_float(), attachs));
      type_check(c, pos, tp, value_type(v), ok);
      break;
    case VCHAR:
      value_set_type(v, tattachs(c, pos, ss, type_new_char(), attachs));
      type_check(c, pos, tp, value_type(v), ok);
      break;
    case VSTR:
      value_set_type(v, tattachs(c, pos, ss, type_new_str(), attachs));
      type_check(c, pos, tp, value_type(v), ok);
      break;
    case VSTR2:
      value_set_type(v, tattachs(c, pos, ss, type_new_str(), attachs));
      type_check(c, pos, tp, value_type(v), ok);
      break;
    case VARR: {
      if (!strcmp(type_id(tp), "Arr"))
        TH2(c, pos) "Expected '%s' but found 'Arr'", type_id(tp) _TH
      EACH(avalue_restore((Arr *)value_data(v)), Value, v2) {
        pvalue(c, ss, atype_get(type_params(tp), 0), v2, type_eq);
      }_EACH
      value_set_type(v, tattachs(c, pos, ss, tp, attachs));
      type_check(c, pos, tp, value_type(v), ok);
      break;
    }
    case VMAP: {
      if (!strcmp(type_id(tp), "Map"))
        TH2(c, pos) "Expected '%s' but found 'Map'", type_id(tp) _TH
      bool is_key = true;
      EACH(avalue_restore((Arr *)value_data(v)), Value, v2) {
        if (is_key) {
          pvalue(c, ss, type_new_str(), v2, type_eq);
          is_key = false;
        } else {
          pvalue(c, ss, atype_get(type_params(tp), 0), v2, type_eq);
          is_key = true;
        }
      }_EACH
      value_set_type(v, tattachs(c, pos, ss, tp, attachs));
      type_check(c, pos, tp, value_type(v), ok);
      break;
    }
    case VFN:
      TH2(c, pos) "Expected '%s' but found a function", type_id(tp) _TH
    default:
      THROW "Unimplemented" _THROW
  }

}

static void pfn(
  Class *c, Lsymbol *ss, Type *tp, Value *v, bool(*ok)(Type *, Type *)
) {

}

static void pany(
  Class *c, Lsymbol *ss, Type *tp, Value *v, bool(*ok)(Type *, Type *)
) {
}

void pvalue(
  Class *c, Lsymbol *ss, Type *tp, Value *v, bool(*ok)(Type *, Type *)
) {
  switch(type_type(tp)) {
    case DATA:
      pdata(c, ss, tp, v, ok);
      break;
    case FN:
      pfn(c, ss, tp, v, ok);
      break;
    case ANY:
      pany(c, ss, tp, v, ok);
      break;
    case VOID:
      THROW "Type is void" _THROW
    case UNKNOWN:
      THROW "Type is unknown" _THROW
  }
}
