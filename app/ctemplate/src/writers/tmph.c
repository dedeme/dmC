// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "writers/tmph.h"
#include "dmc/all.h"

static char *MARK = "/*.-.*/";

static void wrestore(RW *rw, Structure *st) {
  if (st->head->mod != HEAD_SERIAL) {
    return;
  }

  rw_writeln(rw, "");
  rw_writeln(rw, "///");
  rw_writeln(rw, str_printf(
    "%s *%srestore(Json *s);", st->head->id, st->head->prefix
  ));
}

static void wserial(RW *rw, Structure *st) {
  if (st->head->mod == HEAD_NO_SERIAL) {
    return;
  }

  rw_writeln(rw, "");
  rw_writeln(rw, "///");
  rw_writeln(rw, str_printf(
    "Json *%sserialize(%s *this);", st->head->prefix, st->head->id
  ));
}

static void wfuncs(RW *rw, Structure *st) {
  EACH(st->body->ps, Param, p) {
    if (p->mod != PARAM_PRIVATE) {
      rw_writeln(rw, "");
      rw_writeln(rw, "///");
      if (p->func) {
        rw_write(rw, str_replace(
          p->type, "(*)",
          str_printf(
            "(*%s%s(%s *this))", st->head->prefix, p->id, st->head->id
          )
        ));
        rw_writeln(rw, ";");
      } else {
        rw_writeln(rw, str_printf(
          "%s%s%s(%s *this);", p->type, st->head->prefix, p->id, st->head->id
        ));
      }
    }
    if (p->mod == PARAM_SET) {
      rw_writeln(rw, "");
      rw_writeln(rw, "///");
      char *value = str_printf("%svalue", p->type);
      if (p->func) {
        value = str_replace(p->type, "(*)", "(*value)");
      }
      rw_writeln(rw, str_printf(
        "void %sset_%s(%s this, %s);",
        st->head->prefix, p->id, st->head->id, value
      ));
    }
  }_EACH

  EACH(st->body->vs, Var, v) {
    if (v->mod != VAR_PRIVATE) {
      rw_writeln(rw, "");
      rw_writeln(rw, "///");
      rw_writeln(rw, str_printf(
        "%s%s%s(%s *this);", v->type, st->head->prefix, v->id, st->head->id
      ));
    }
    if (v->mod == VAR_SET) {
      rw_writeln(rw, "");
      rw_writeln(rw, "///");
      rw_writeln(rw, str_printf(
        "void %sset_%s(%s this, %s);",
        st->head->prefix, v->id, st->head->id, str_printf("%svalue", v->type)
      ));
    }
  }_EACH

  EACH(st->body->fs, Func, f) {
    if (f->mod != FUNC_PRIVATE) {
      rw_writeln(rw, "");
      rw_writeln(rw, "///");
      rw_write(rw, str_replace(
        f->type, "(*)",
        str_printf(
          "(*%s%s(%s *this))", st->head->prefix, f->id, st->head->id
        )
      ));
      rw_writeln(rw, ";");
    }
    if (f->mod == FUNC_SET) {
      rw_writeln(rw, "");
      rw_writeln(rw, "///");
      char *value = str_replace(f->type, "(*)", "(*value)");
      rw_writeln(rw, str_printf(
        "void %sset_%s(%s this, %s);",
        st->head->prefix, f->id, st->head->id, value
      ));
    }
  }_EACH
}

static void wnew(RW *rw, Structure *st) {
  if (st->head->id_mod == HEAD_ID_PRIVATE) {
    return;
  }

  rw_writeln(rw, "");
  rw_writeln(rw, "///");
  size_t size = arr_size(st->body->ps);
  if (size > 3) {
    rw_write(rw, str_printf(
      "%s *%snew(",
      st->head->id, st->head->prefix
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
    rw_writeln(rw, "\n);");
  } else {
    rw_write(rw, str_printf(
      "%s *%snew(",
      st->head->id, st->head->prefix
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
    rw_writeln(rw, ");");
  }
}

static void wtypedef(RW *rw, Structure *st) {
  if (st->head->id_mod != HEAD_ID_PRIVATE) {
    rw_writeln(rw, "");
    rw_writeln(rw, "///");
    rw_writeln(rw, str_printf(
      "typedef struct %s%s %s;",
      st->head->prefix, st->head->id, st->head->id
    ));
  }
}

static void wstructure(RW *rw, Structure *st) {
  wtypedef(rw, st);
  wnew(rw, st);
  wfuncs(rw, st);
  wserial(rw, st);
  wrestore(rw, st);
}

void tmph_write(Template *tp, RW *rw) {
  rw_read_until(rw, MARK);

  EACH(tp->sts, Structure, st) {
    wstructure(rw, st);
    rw_writeln(rw, "");
  }_EACH

  rw_read_blank(rw, MARK);
  rw_read_end(rw);
}

