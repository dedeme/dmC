// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "writers/funcs.h"
#include "dmc/all.h"

void funcs_write(RW *rw, Structure *st) {
  EACH(st->body->ps, Param, p) {
    if (p->mod != PARAM_PRIVATE) {
      rw_writeln(rw, "");
      rw_writeln(rw, "inline");
      if (p->func) {
        rw_write(rw, str_replace(
          p->type, "(*)",
          str_printf(
            "(*%s%s(%s *this))", st->head->prefix, p->id, st->head->id
          )
        ));
        rw_writeln(rw, " {");
      } else {
        rw_writeln(rw, str_printf(
          "%s%s%s(%s *this) {", p->type, st->head->prefix, p->id, st->head->id
        ));
      }
      rw_writeln(rw, str_printf("  return this->%s;", p->id));
      rw_writeln(rw, "}");
    }
    if (p->mod == PARAM_SET) {
      rw_writeln(rw, "");
      rw_writeln(rw, "inline");
      char *value = str_printf("%svalue", p->type);
      if (p->func) {
        value = str_replace(p->type, "(*)", "(*value)");
      }
      rw_writeln(rw, str_printf(
        "void %sset_%s(%s this, %s) {",
        st->head->prefix, p->id, st->head->id, value
      ));
      rw_writeln(rw, str_printf("  this->%s = value;", p->id));
      rw_writeln(rw, "}");
    }
  }_EACH

  EACH(st->body->vs, Var, v) {
    if (v->mod != VAR_PRIVATE) {
      rw_writeln(rw, "");
      rw_writeln(rw, "inline");
      rw_writeln(rw, str_printf(
        "%s%s%s(%s *this) {", v->type, st->head->prefix, v->id, st->head->id
      ));
      rw_writeln(rw, str_printf("  return this->%s;", v->id));
      rw_writeln(rw, "}");
    }
    if (v->mod == VAR_SET) {
      rw_writeln(rw, "");
      rw_writeln(rw, "inline");
      rw_writeln(rw, str_printf(
        "void %sset_%s(%s this, %s) {",
        st->head->prefix, v->id, st->head->id, str_printf("%svalue", v->type)
      ));
      rw_writeln(rw, str_printf("  this->%s = value;", v->id));
      rw_writeln(rw, "}");
    }
  }_EACH

  EACH(st->body->fs, Func, f) {
    if (f->mod != FUNC_PRIVATE) {
      rw_writeln(rw, "");
      rw_writeln(rw, "inline");
      rw_write(rw, str_replace(
        f->type, "(*)",
        str_printf(
          "(*%s%s(%s *this))", st->head->prefix, f->id, st->head->id
        )
      ));
      rw_writeln(rw, " {");
      rw_writeln(rw, str_printf("  return this->%s;", f->id));
      rw_writeln(rw, "}");
    }
    if (f->mod == FUNC_SET) {
      rw_writeln(rw, "");
      rw_writeln(rw, "inline");
      char *value = str_replace(f->type, "(*)", "(*value)");
      rw_writeln(rw, str_printf(
        "void %sset_%s(%s this, %s) {",
        st->head->prefix, f->id, st->head->id, value
      ));
      rw_writeln(rw, str_printf("  this->%s = value;", f->id));
      rw_writeln(rw, "}");
    }
  }_EACH
}
