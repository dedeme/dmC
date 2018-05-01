// Copyright 13-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "parser/parser.h"
#include "global.h"
#include "lexer/Txpos.h"
#include "ast/Class.h"
#include "ast/Stat.h"
#include "ast/Attach.h"
#include "parser/Final_class.h"
#include "js/natbool.h"
#include "js/natbyte.h"
#include "js/natint.h"
#include "js/natfloat.h"
#include "js/natchar.h"
#include "js/natstr.h"
#include "js/natarr.h"
#include "js/natmap.h"

Obj *parser_class_obj(char *pack, Cpath *path, char *object) {
  Final_class *fc = NULL;
  EACH(global_final_classes(), Final_class, c) {
    if (cpath_eq(final_class_cpath(c), path)) {
      fc = c;
      break;
    }
  }_EACH

  if (!fc) {
    fc = final_class_new(path);
    arr_add(global_final_classes(), fc);
  }

  bool local_callable = true;
  if (!final_class_public(fc) && strcmp(pack, cpath_parent(path))) {
    local_callable = false;
  }
  if (!final_class_public(fc) && !local_callable)
    TH2(class_mk(final_class_cpath(fc)), pos_new(0, 0))
      "'%s' is private and only can be called from its own directory",
      cpath_id(final_class_cpath(fc))
    _TH

  return final_class_object(fc, object);
}

// Attachs -----------------------------------------------------------
// -------------------------------------------------------------------

static Type *adot(Final_class *class, Type *tp, Attach *a) {
  switch (type_type(tp)) {
  case DATA: {
    char *id = type_id(tp);
    if (!strcmp(id, "Bool")) {
      return obj_type(natbool_instance(class, attach_pos(a), attach_id(a)));
    } else if (!strcmp(id, "Byte")) {
      return obj_type(natbyte_instance(class, attach_pos(a), attach_id(a)));
    } else if (!strcmp(id, "Int")) {
      return obj_type(natint_instance(class, attach_pos(a), attach_id(a)));
    } else if (!strcmp(id, "Float")) {
      return obj_type(natfloat_instance(class, attach_pos(a), attach_id(a)));
    } else if (!strcmp(id, "Char")) {
      return obj_type(natchar_instance(class, attach_pos(a), attach_id(a)));
    } else if (!strcmp(id, "Str")) {
      return obj_type(natstr_instance(class, attach_pos(a), attach_id(a)));
    } else {
      return obj_type(parser_class_obj(
        cpath_parent(final_class_cpath(class)), cpath_from_id(id), attach_id(a)
      ));
    }
  }
  case ARR:
    return obj_type(natarr_instance(class, attach_pos(a), attach_id(a)));
  case MAP:
    return obj_type(natmap_instance(class, attach_pos(a), attach_id(a)));
  case FN:
    TH3(class, attach_pos(a)) "Functions do not take point operator" _TH
  case ANY:
    TH3(class, attach_pos(a)) "Any does not take point operator" _TH
  case UNKNOWN:
    TH3(class, attach_pos(a)) "Type is unknown" _TH
  default:
    THROW "Type '%d' is unknown", type_type(tp) _THROW
  }
  return NULL;
}

static Type *asub(Final_class *class, Lobjs *objs, Type *tp, Attach *a) {
  Value *v = arr_get(attach_values(a), 0);

  switch (type_type(tp)) {
  case ARR: {
    parser_value_type(class, objs, type_new_int(), v);
    return arr_get(type_params(tp), 0);
  }
  case MAP: {
    parser_value_type(class, objs, type_new_str(), v);
    return arr_get(type_params(tp), 0);
  }
  default:
    TH3(class, attach_pos(a))
      "Value subscripted is expected to be an Array o Map"
    _TH
  }
  return NULL;
}

static Type *afn(Final_class *class, Lobjs *objs, Type *tp, Attach *a) {
  if (type_type(tp) != FN)
    TH3(class, attach_pos(a)) "Expected a function" _TH

  Arr/*Type*/ *tps = type_params(tp);
  Arr/*Value*/ *vs = attach_values(a);
  if (arr_size(vs) != arr_size(tps) - 1)
    TH3(class, attach_pos(a))
      "Expected '%d' parameters, but found '%d'",
      arr_size(tps) - 1, arr_size(vs)
    _TH

  RANGE0(i, arr_size(vs)) {
    parser_value_type(class, objs, arr_get(tps, i), arr_get(vs, i));
  }_RANGE
  return arr_nget(tps, arr_size(vs));
}

// tp is the expected type of 'value'
static Type *attachs(Final_class *class, Lobjs *objs, Type *tp, Value *value) {
  EACH(value_attachs(value), Attach, a) {
    switch (attach_type(a)) {
    case ADOT: // tp must be ARR, MAP or DATA.
      tp = adot(class, tp, a);
      break;
    case ASUB: // tp must be ARR or MAP
      tp = asub(class, objs, tp, a);
      break;
    case AFN: // tp must be FN
      tp = afn(class, objs, tp, a);
      break;
    default:
      THROW "Atype '%d' is unknown", attach_type(a) _THROW
    }
  }_EACH
  return tp;
}

// Values ------------------------------------------------------------
// -------------------------------------------------------------------

static Type *tnull(Final_class *class, Type *tp, Pos *pos) {
  if (type_is_unknown(tp))
    TH3(class, pos) "Type is unknown" _TH
  return tp;
}

static Type *tcommon(Final_class *class, Lobjs *objs, Type *tp, Value *value) {
  if (!tp)
    TH3(class, value_pos(value)) "Espected type %s", type_to_str(tp) _TH

  Type *tpv = attachs(class, objs, value_type(value), value);
  if (type_is_unknown(tp)) {
    return tpv;
  }

  if (!type_child(tp, tpv))
    TH3(class, value_pos(value)) "Espected type %s", type_to_str(tp) _TH

  return tp;
}

static Type *tbyte(Final_class *class, Type *tp, Pos *pos) {
  if (!tp || (!type_is_unknown(tp) && !type_child(tp, type_new_byte())))
    TH3(class, pos) "Espected type %s", type_to_str(tp)   _TH
  return type_new_byte();
}

static Type *tint(Final_class *class, Type *tp, Pos *pos) {
  if (!tp || (!type_is_unknown(tp) && !type_child(tp, type_new_int())))
    TH3(class, pos) "Espected type %s", type_to_str(tp)   _TH
  return type_new_int();
}

static Type *tfloat(Final_class *class, Type *tp, Pos *pos) {
  if (!tp || (!type_is_unknown(tp) && !type_child(tp, type_new_float())))
    TH3(class, pos) "Espected type %s", type_to_str(tp)   _TH
  return type_new_float();
}

static Type *tarr(Final_class *class, Lobjs *objs, Type *tp, Value *value) {
//  Type *atype = arr_get(type_params(tp), 0);
  printf("-->%s\n", type_to_str(tp));
  EACH(value_values(value), Value, v) {
    puts(type_to_str(value_type(v)));
  }_EACH
  return tp;
}

static Type *tid(Final_class *class, Lobjs *objs, Type *t, Value *value) {
  Pos *pos = value_pos(value);
  char *id = arr_get(value_ids(value), 0);
  Lobj *obj = lobjs_get(objs, id);
  Type *tp;
  if (obj) {
    tp = lobj_type(obj);
  } else {
    Obj *o = final_class_object(class, id);
    if (!obj_is_static(o))
      TH3(class, pos) "Expected a static value" _TH
    arr_set(value_ids(value), 0, obj_absolute_id(o));
    tp = obj_type(o);
  }

  tp = attachs(class, objs, tp, value);

  if (type_is_unknown(t)) {
    return tp;
  }

  if (!type_child(t, tp))
    TH3(class, pos) "Espected type %s", type_to_str(t) _TH

  return tp;
}

static Type *tfn(Final_class *class, Lobjs *objs, Type *tp, Value *v) {
  Pos *pos = value_pos(v);
  if (type_is_unknown(tp))
    TH3(class, pos) "Type is unknown" _TH
  if (type_type(tp) != FN)
    TH3(class, pos) "Expected a function" _TH

  Arr/*char*/ *ids = value_ids(v);
  Arr/*Type*/ *ptypes = type_params(tp);
  if (arr_size(ids) != arr_size(ptypes) - 1)
    TH3(class, pos)
      "Expected %d parameters but function has %d",
      arr_size(ptypes) - 1, arr_size(ids)
    _TH

  RANGE0(i, arr_size(ids)) {
    objs = lobjs_add(objs,
      lobj_new(arr_get(ids, i), class, arr_get(ptypes, i), pos, false));
  }_RANGE

  parser_block(class, objs, arr_nget(ptypes, arr_size(ids)), value_stats(v));

  return tp;
}

Type *parser_value_type(Final_class *class, Lobjs *objs, Type *tp, Value *v) {
  switch (value_vtype(v)) {
  case VNULL: return tnull(class, tp, value_pos(v));
  case VBOOL:
  case VCHAR:
  case VSTR:
  case VSTR2:
  case VMAP: return tcommon(class, objs, tp, v);
  case VBYTE: return tbyte(class, tp, value_pos(v));
  case VINT: return tint(class, tp, value_pos(v));
  case VFLOAT: return tfloat(class, tp, value_pos(v));
  case VARR: return tarr(class, objs, tp, v);
  case VID: return tid(class, objs, tp, v);
  case VFN: return tfn(class, objs, tp, v);
  default:
    THROW "Vtype '%d' is unknown", value_vtype(v) _THROW
  }
  return NULL;
}

// Stats -------------------------------------------------------------
// -------------------------------------------------------------------

static void sfn(Final_class *class, Lobjs *objs, Stat *st) {
  Type *tp = tid(class, objs, NULL, arr_get(stat_values(st), 0));
  if (tp)
    TH3(class, stat_pos(st)) "Expected a statement buf a value was found" _TH
}

void parser_block(
  Final_class *class, Lobjs *objs, Type *tp, Arr/*Stat*/ *stats
) {
  bool returned = false;

  EACH(stats, Stat, st) {
    switch(stat_stype(st)) {
    case SFN:
      sfn(class, objs, st);
      break;
    case SNATIVE:
      break;
    default:
      THROW "Stype '%d' is unknown", stat_stype(st) _THROW
    }
  }_EACH

  if (tp && !returned)
    TH3(class, stat_pos(arr_get(stats, arr_size(stats) - 1)))
      "Return is missing"
    _TH
}
