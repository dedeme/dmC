// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "jstemplate.h"
#include <dm/dm.h>
#include <ctype.h>

struct Par {
  bool sta;
  bool writing;
  char *id;
  char *tp;
};

static struct Par *par_new(
  bool sta,
  bool writing,
  char *id,
  char *tp
) {
  struct Par *this = MALLOC(struct Par);
  this->sta = sta;
  this->writing = writing;
  this->id = id;
  this->tp = tp;
  return this;
}

static Arr/*Par*/ *pars;

static char *cut(char *s, uint ix) {
  return str_trim(str_sub_end(s, ix));
}

static void help() {
	puts(
		"Use:\n"
		"  jstemplate file\n"
		"For example:\n"
		"  jstemplate \"src/main.js\"\n"
  );
}

static char *t_class(char *name, Arr/*char*/ *lines) {
  struct Field {
		char *id;
		bool writing;
		char *tp;
	};
  void field_new(struct Field **r, char *id, bool writing, char *tp) {
    struct Field *this = MALLOC(struct Field);
    this->id = id;
    this->writing = writing;
    this->tp = tp;
    *r = this;
  }

  Arr/*Field*/ *fields = arr_new();

  EACH(lines, char, l) {
    if (*l) {
			bool writing = false;
			if (*l == '@') {
				writing = true;
				++l;
			}
			int ix = str_cindex(l, ':');
			if (ix == -1) {
				THROW "':' is missing in '%s'", l _THROW
			}
      char *id = str_trim(str_sub(l, 0, ix));
      char *tp = str_trim(str_sub_end(l, ix + 1));

      if (!*id) {
				THROW "Field name is missing in '%s'", l _THROW
      }
      if (!*tp) {
				THROW "Field type is missing in '%s'", l _THROW
      }
      struct Field *field;
      field_new(&field, id, writing, tp);
      arr_add(fields, field);
    }
  }_EACH
	if (!arr_size(fields)) {
    THROW "There are no fields in 'class'" _THROW
	}

  Buf *bf = buf_new();

  buf_add(bf, name);
  buf_add(bf, " = class {\n  /**\n");
  EACH(fields, struct Field, f) {
    buf_add(bf, "   * @param {");
    buf_add(bf, f->tp);
    buf_add(bf, "} ");
    buf_add(bf, f->id);
    buf_add(bf, "\n");
  }_EACH
  buf_add(bf, "   */\n");

  buf_add(bf, "  constructor (");
  buf_add(bf, ((struct Field *)arr_get(fields, 0))->id);
  RANGE(i, 1, arr_size(fields)) {
    buf_add(bf, ", ");
    buf_add(bf, ((struct Field *)arr_get(fields, i))->id);
  }_RANGE
  buf_add(bf, ") {\n");

  EACH(fields, struct Field, f) {
    buf_add(bf, "    /** @private */\n    this._");
    buf_add(bf, f->id);
    buf_add(bf, " = ");
    buf_add(bf, f->id);
    buf_add(bf, ";\n");
  }_EACH
  buf_add(bf, "  }\n");

  EACH(fields, struct Field, f) {
    buf_add(bf, "\n  /** @return {");
    buf_add(bf, f->tp);
    buf_add(bf, "} */\n  ");
    buf_add(bf, f->id);
    buf_add(bf, " () {\n    return this._");
    buf_add(bf, f->id);
    buf_add(bf, ";\n  }\n");

    if (f->writing) {
      buf_add(bf, "\n  /** @param {");
      buf_add(bf, f->tp);
      buf_add(bf, "} value */\n  set");

      char tmp = *f->id;
      *f->id = toupper(tmp);
      buf_add(bf, f->id);
      *f->id = tmp;

      buf_add(bf, " (value) {\n    this._");
      buf_add(bf, f->id);
      buf_add(bf, " = value;\n  }\n");
    }
  }_EACH

  buf_add(bf, "\n  /** @return {!Array<?>} */\n");
  buf_add(bf, "  serialize () {\n");
  buf_add(bf, "    return [\n");
  buf_add(bf, "      this._");
  buf_add(bf, ((struct Field *)arr_get(fields, 0))->id);
  RANGE(i, 1, arr_size(fields)) {
    buf_add(bf, ",\n      this._");
    buf_add(bf, ((struct Field *)arr_get(fields, i))->id);
  }_RANGE
  buf_add(bf, "\n    ];\n  }\n");

  buf_add(bf, "\n  /**\n");
  buf_add(bf, "   * @param {!Array<?>} serial\n");
  buf_add(bf, "   * @return {!");
  buf_add(bf, name);
  buf_add(bf, "}\n");
  buf_add(bf, "   */\n");
  buf_add(bf, "  static restore (serial) {\n");
  buf_add(bf, "    return new ");
  buf_add(bf, name);
  buf_add(bf, " (\n");
  buf_add(bf, "      serial[0]");
  RANGE(i, 1, arr_size(fields)) {
    buf_add(bf, ",\n      serial[");
    buf_add(bf, str_printf("%d", i));
    buf_add(bf, "]");
  }_RANGE

  buf_add(bf, "\n    );\n  }\n}\n");

  return buf_to_str(bf);
}

static char *t_func(char *kind, char *name, Arr/*char*/ *lines) {
  Buf *cm = buf_new();
  Buf *js = buf_new();

  buf_add(cm, "  /**\n");

  buf_add(js, "  static ");
  if (!strcmp(kind, "func")) {
    buf_add(js, "  function ");
  } else if (!strcmp(kind, "meth")) {
    buf_add(js, "  " );
  }
  buf_add(js, name);
  buf_add(js, " (");

  bool with_return = false;
  int i = 0;
  for (; i < arr_size(lines); ++i) {
    char *l = arr_get(lines, i);
    if (*l) {
      int ix = str_cindex(l, ':');
      if (ix == -1) {
        buf_add(cm, "   * @return {");
        buf_add(cm, l);
        buf_add(cm, "}\n");
        ++i;
        with_return = true;
        break;
      }

      char *id = str_trim(str_sub(l, 0, ix));
      char *tp = str_trim(str_sub_end(l, ix + 1));

      if (!*id) {
				THROW "Field name is missing in '%s'", l _THROW
      }
      if (!*tp) {
				THROW "Field type is missing in '%s'", l _THROW
      }
      buf_add(cm, "   * @param {");
      buf_add(cm, tp);
      buf_add(cm, "} ");
      buf_add(cm, id);
      buf_add(cm, "\n");

      buf_add(js, id);
      buf_add(js, ", ");

      free(tp);
      free(id);
    }

  }

  for (; i< arr_size(lines); ++i) {
    if (*(char *)arr_get(lines, i)) {
      THROW "Field after return in '%s'", (char *)arr_get(lines, i) _THROW
    }
  }

  if (str_ends(buf_str(js), ", ")) {
    Buf *new_bf = buf_new();
    buf_add_buf(new_bf, buf_str(js), buf_length(js) - 2);
    js = new_bf;
  }

  if (with_return) {
    buf_add(cm, "   * @return {void}\n");
  }

  buf_add(cm, "   */\n");
  buf_add(js, ") {\n  }\n");

  buf_add(cm, buf_str(js));
  return buf_to_str(cm);
}

static char *t_vars(char *kind, Arr/*char*/ *lines) {
  Buf *bf = buf_new();

  bool sta = false;
  if (!strcmp(kind, "pars")) {
    sta = true;
  }

  EACH(lines, char, l) {
    if (*l) {
      bool writing = false;
      if (*l == '@') {
        if (!strcmp(kind, "consts")) {
          THROW "Value constant with '@' in '%s'", l _THROW
        }
        writing = true;
        ++l;
      }
      int ix = str_cindex(l, ':');
			if (ix == -1) {
				THROW "':' is missing in '%s'", l _THROW
			}
      char *id = str_trim(str_sub(l, 0, ix));
      char *tp = str_trim(str_sub_end(l, ix + 1));

      if (!*id) {
				THROW "Field name is missing in '%s'", l _THROW
      }
      if (!*tp) {
				THROW "Field type is missing in '%s'", l _THROW
      }

      struct Par *par = par_new(sta, writing, id, tp);
      arr_add(pars, par);

      char *val = "null";
      if (!strcmp(tp, "boolean")) {
        val = "false";
      } else if (!strcmp(tp, "number")) {
        val = "0";
      } else if (!strcmp(tp, "string")) {
        val = "\"\"";
      }

      if (!strcmp(kind, "vars")) {
        buf_add(bf, "  /** @type {");
        buf_add(bf, tp);
        buf_add(bf, "} */\n  let ");
      } else if (!strcmp(kind, "consts")) {
        buf_add(bf, "  /** @const {");
        buf_add(bf, tp);
        buf_add(bf, "} */\n  const ");
      } else {
        buf_add(bf, "    /**\n     * @private\n     * @type {");
        buf_add(bf, tp);
        buf_add(bf, "}\n     */\n    this._");
      }
      buf_add(bf, id);
      buf_add(bf, " = ");
      buf_add(bf, val);
      buf_add(bf, ";\n");
    }
  }_EACH
  buf_add(bf, "\n");

  return buf_to_str(bf);
}

static char *t_links(Arr/*char*/ *lines) {
  EACH(lines, char, l) {
    if (*l) {
      THROW "Value in links '%s'", l _THROW
    }
  }_EACH

  Buf *bf = buf_new();

  EACH(pars, struct Par, p) {
    buf_add(bf, "  /** @return {");
    buf_add(bf, p->tp);
    buf_add(bf, "} */\n  ");

    if (p->sta) {
      buf_add(bf, "static ");
    }

    buf_add(bf, p->id);
    buf_add(bf, " () {\n    return ");
    if (!p->sta) {
      buf_add(bf, "this._");
    }
    buf_add(bf, p->id);
    buf_add(bf, ";\n  }\n\n");

    if (p->writing) {
      char tmp = *p->id;
      *p->id = toupper(*p->id);
      buf_add(bf, "  /**\n   * @param {");
      buf_add(bf, p->tp);
      buf_add(bf, "} value\n   * @return {void}\n   */\n  ");
      if (p->sta) {
        buf_add(bf, "static ");
      }
      buf_add(bf, "set");
      buf_add(bf, p->id);
      buf_add(bf, " (value) {\n    ");
      *p->id = tmp;
      if (!p->sta) {
        buf_add(bf, "this._");
      }
      buf_add(bf, p->id);
      buf_add(bf, " = value;\n  }\n\n");
    }
  }_EACH

  pars = arr_new();
	return buf_to_str(bf);
}

int main (int argc, char **argv) {
  exc_init();

  if (argc != 2) {
    puts("Wrong number of parameters calling jstmplate");
    help();
    return 0;
  }

  pars = arr_new();

  char *jsfile = argv[1];
  char *tmpfile = file_tmp("jstemplate");
  LckFile *wlck = file_wopen(tmpfile);

  bool changes = false;
  Arr/*char*/ *lines = arr_new();
  char *template_type = "";
  char *template_name = "";

  LckFile *rlck = file_ropen(jsfile);
  char *l = file_read_line(rlck);
  while (*l) {
    char *ltrim = str_trim(l);
    if (*template_type) {
			if (strcmp(ltrim, ">>>")) {
        arr_add(lines, ltrim);
      } else {
        free(ltrim);
        char *code;
        if (!strcmp(template_type, "class")) {
          code = t_class(template_name, lines);
        } else if (!strcmp(template_type, "func")) {
          code = t_func("func", template_name, lines);
        } else if (!strcmp(template_type, "meth")) {
          code = t_func("meth", template_name, lines);
        } else if (!strcmp(template_type, "static")) {
          code = t_func("static", template_name, lines);
        } else if (!strcmp(template_type, "vars")) {
          code = t_vars("vars", lines);
        } else if (!strcmp(template_type, "consts")) {
          code = t_vars("consts", lines);
        } else if (!strcmp(template_type, "pars")) {
          code = t_vars("pars", lines);
        } else if (!strcmp(template_type, "links")) {
          code = t_links(lines);
        } else {
          THROW "case '%s' is unknown", template_type _THROW
        }

        file_write_text(wlck, code);

        lines = arr_new();
        template_type = "";
        template_name = "";
      }
    } else {
      if (str_starts(ltrim, "<<<class:")) {
        template_type = "class";
        template_name = cut(ltrim, 9);
        changes = true;
      } else if (str_starts(ltrim, "<<<func:")) {
        template_type = "func";
        template_name = cut(ltrim, 8);
        changes = true;
      } else if (str_starts(ltrim, "<<<meth:")) {
        template_type = "meth";
        template_name = cut(ltrim, 8);
        changes = true;
      } else if (str_starts(ltrim, "<<<static:")) {
        template_type = "static";
        template_name = cut(ltrim, 8);
        changes = true;
      } else if (str_starts(ltrim, "<<<vars:")) {
        template_type = "vars";
        changes = true;
      } else if (str_starts(ltrim, "<<<consts:")) {
        template_type = "consts";
        changes = true;
      } else if (str_starts(ltrim, "<<<pars:")) {
        template_type = "pars";
        changes = true;
      } else if (str_starts(ltrim, "<<<links:")) {
        template_type = "links";
      } else {
        file_write_text(wlck, l);
      }
    }

    l = file_read_line(rlck);
  }
  file_close(rlck);
  file_close(wlck);

  if (changes) {
    file_copy(tmpfile, jsfile);
  }
  file_del(tmpfile);

  return 0;
}
