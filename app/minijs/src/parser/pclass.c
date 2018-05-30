// Copyright 16-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "parser/pclass.h"
#include "ast/Program.h"
#include "lexer/Tx.h"
#include "lexer/rclass.h"
#include "DEFS.h"

static void generics_check(Class *c, Atype *generics) {
  if (achar_size(class_generics(c)) != arr_size(generics))
    TH2(c, pos_new(0, 0))
      "'%s' is called with %d generics, but it has '%d'",
      class_id(c), arr_size(generics), achar_size(class_generics(c))
    _TH
}

static char *mk_chain(Achar *children, char *super) {
  Buf *bf = buf_new();
  EACH((Arr *)children, char, child) {
    buf_add(bf, child);
    buf_add(bf, " -> ");
  }_EACH
  buf_add(bf, super);
  return buf_to_str(bf);
}

static void super_check2(Class *c, Achar *children, Type *super) {
  char *super_id = type_id(super);
  EACH((Arr *)children, char, child) {
    if (!strcmp(child, super_id))
      TH2(c, pos_new(0,0))
        "Cyclic inheritance: %s", mk_chain(children, super_id)
      _TH
  }_EACH

  Class *csuper = program__class(program_get(), super_id);
  if (!csuper)
    TH2(c, pos_new(0,0))
      "Unknown class '%s' in inheritance chain: %s",
      super_id, mk_chain(children, super_id)
    _TH

  if (achar_size(class_generics(csuper)) != arr_size(type_params(super))) {
    char *child = achar_get(children, achar_size(children) - 1);
    TH2(c, pos_new(0,0))
      "'%s' is extended from '%s' using %d generics, when '%s' needs %d",
      child, super_id, arr_size(type_params(super)),
      child, achar_size(class_generics(csuper))
    _TH
  }

  Type *super2 = class_super(csuper);
  if (type_type(super2) != VOID) {
    achar_add(children, super_id);
    super_check2(c, children, super2);
  }
}

static void super_check(Class *c) {
  Type *super = class_super(c);

  if (type_type(super) != VOID) {
    Achar *children = achar_new();
    achar_add(children, class_id(c));
    super_check2(c, children, super);
  }
}

static void atts_check(Class *c) {
  Aatt *atts = class_statics(c);
  EACH((Arr *)atts, Att, a) {
    Value *v = att_value(a);
    aatt_set(atts, _i, att_new(
      att_is_public(a), att_id(a),
      class_actual_type(c, att_type(a), value_pos(v)),
      att_is_val(a), v
    ));
  }_EACH

  atts = class_instance(c);
  EACH((Arr *)atts, Att, a) {
    Value *v = att_value(a);
    aatt_set(atts, _i, att_new(
      att_is_public(a), att_id(a),
      class_actual_type(c, att_type(a), value_pos(v)),
      att_is_val(a), v
    ));
  }_EACH

}

void pclass2(Class *c, Atype *generics) {
  generics_check(c, generics);
  super_check(c);
  atts_check(c);   // modify attributes type

}

void pclass_main(Class *c, Atype *generics) {
  if (class_local(c))
    TH2(c, pos_new(0,0)) "Main class can not be _local" _TH
  pclass2(c, generics);
}

void pclass(Class *caller, Class *c, Atype *generics) {
  if (class_local(c)) {
    Cpath *callerp = cpath_from_id(class_id(caller));
    Cpath *cp = cpath_from_id(class_id(c));
    if (strcmp(cpath_parent(callerp), cpath_parent(cp)))
      TH2(c, pos_new(0, 0))
        "'%s' is a _local class and can not be called from '%s'",
        class_id(c), class_id(caller)
      _TH
  }
  pclass2(c, generics);
}

void pclass_implementation(Class *c, Atype *generics) {
  Aatt *this_atts = class_instance_all(c, generics);

  EACH(class_implements(c), Type, tp) {
    Type *atp = class_actual_type(c, tp, pos_new(0,0));
    Class *i = program__class(program_get(), type_id(atp));
    if (!i) {
      i = rclass(cpath_from_id(type_id(atp)));
      pclass(c, i, generics);
      program_add(program_get(), i);
      pclass_implementation(i, generics);
    }

    EACH((Arr *)class_instance_all(i, type_params(atp)), Att, a) {
      bool is_in = false;
      EACH((Arr *)this_atts, Att, this_a) {
        if (!strcmp(att_id(this_a), att_id(a))) {
          if (!type_eq(att_type(this_a), att_type(a)))
            TH2(c, pos_new(0,0))
              "%s.%s does not implement %s.%s because their type is different",
              class_id(c), att_id(a), type_id(atp), att_id(a)
            _TH
          is_in = true;
          break;
        }
      }_EACH

      if (!is_in)
        TH2(c, pos_new(0,0))
          "%s does not implement %s because it does not have the attribute %s",
          class_id(c), type_id(atp), att_id(a)
        _TH
    }_EACH

  }_EACH
}
