// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "writers/new.h"
#include "dmc/all.h"

void new_write(RW *rw, Structure *st) {
  rw_writeln(rw, "");

  char *private_mark = st->head->id_mod == HEAD_ID_PUBLIC ? "_" : "";

  size_t size = arr_size(st->body->ps);
  if (size > 3) {
    rw_write(rw, str_printf(
      "%s *%s%snew(",
      st->head->id, private_mark, st->head->prefix
    ));

    bool first = true;
    EACH(st->body->ps, Param, p) {
      if (first) first = false;
      else rw_write(rw, ",");

      if (p->func) {
        rw_write(rw, str_printf(
          "\n  %s", str_replace(p->type, "(*)", str_printf("(*%s)", p->id))
        ));
      } else {
        rw_write(rw, str_printf("\n  %s%s", p->type, p->id));
      }
    }_EACH
    rw_writeln(rw, "\n) {");
  } else {
    rw_write(rw, str_printf(
      "%s *%s%snew(",
      st->head->id, private_mark, st->head->prefix
    ));

    bool first = true;
    EACH(st->body->ps, Param, p) {
      if (first) first = false;
      else rw_write(rw, ", ");

      if (p->func) {
        rw_write(rw, str_replace(p->type, "(*)", str_printf("(*%s)", p->id)));
      } else {
        rw_write(rw, str_printf("%s%s", p->type, p->id));
      }
    }_EACH
    rw_writeln(rw, ") {");
  }

  rw_writeln(rw, str_printf(
    "  %s *this = MALLOC(%s);", st->head->id, st->head->id
  ));
  EACH(st->body->ps, Param, p) {
    rw_writeln(rw, str_printf(
      "  this->%s = %s;", p->id, p->id
    ));
  }_EACH
  EACH(st->body->vs, Var, v) {
    rw_writeln(rw, str_printf(
      "  this->%s = %s;", v->id, v->value
    ));
  }_EACH
  EACH(st->body->fs, Func, f) {
    rw_writeln(rw, str_printf(
      "  this->%s = %s;", f->id, f->id
    ));
  }_EACH

  rw_writeln(rw, "  return this;");
  rw_writeln(rw, "}");

}
