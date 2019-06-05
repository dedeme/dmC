// Copyright 12-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "record.h"
#include "ctype.h"

static char *cname = NULL;
static int toJs = 0;
static int fromJs = 0;
static char *doc_null = NULL;
// Arr[struct Field]
static Arr *fields = NULL;


static int is_simple_type (char *tp) {
  char *simple_types = " boolean ?boolean number ?number string ?string ";
  char *t = str_f(" %s ", tp);
  int r = str_index(simple_types, t) != -1;
  return r;
}

static int is_array (char *tp) {
  return *tp == '!' ? str_eq(tp, "!Array") : str_eq(tp, "Array");
}

static int is_map (char *tp) {
  return *tp == '!' ? str_eq(tp, "!Map") : str_eq(tp, "Map");
}

struct Field {
  char *doc_null;
  int writable; // 'set' allowed
  char *name;
  char *tp1;
  char *tp2_null;
};

static struct Field *field_new(
  char *name, char *tp1, char *tp2_null
) {
  struct Field *this = MALLOC(struct Field);
  this->doc_null = doc_null ? str_new(doc_null) : NULL;
  doc_null = NULL;
  this->writable = *name == '+';
  char *nm = str_new(name);
  if (this->writable) {
    char *tmp = nm;
    nm = str_right(tmp, 1);
  }
  this->name = nm;
  this->tp1 = str_new(tp1);
  this->tp2_null = tp2_null ? str_new(tp2_null) : NULL;
  return this;
}

static char *field_type(struct Field *this) {
  Buf *bf = buf_new();
  buf_cadd(bf, '{');
  buf_add(bf, this->tp1);
  if (this->tp2_null) {
    buf_cadd(bf, '<');
    if (is_map(this->tp1)) buf_add(bf, "string, ");
    buf_add(bf, this->tp2_null);
    buf_cadd(bf, '>');
  }
  buf_cadd(bf, '}');
  return buf_to_str(bf);
}

char *record_init(char *line) {
  char *l = str_right(line, 4);
  // Arr[char]
  Arr *ps = str_csplit_trim(l, ':');
  int pslen = arr_size(ps);

  if (pslen > 2) {
    return str_new("Unexpected second ':'");
  }

  if (pslen == 0 || !*((char *)arr_get(ps, 0))) {
    return str_new("Class name is mising");
  }

  toJs = 0;
  fromJs = 0;

  cname = str_new(arr_get(ps, 0));

  if (pslen == 2) {
    l = arr_get(ps, 1);
    if (!*l) {
      return str_new("Expected 'to|from|serial'");
    } else if (str_eq(l, "to")) {
      toJs = 1;
    } else if (str_eq(l, "from")) {
      fromJs = 1;
    } else if (str_eq(l, "serial")) {
      toJs = 1;
      fromJs = 1;
    } else {
      char *r = str_f("Expected 'to|from|serial'. Found '%s'", l);
      return r;
    }
  }

  doc_null = NULL;
  fields = arr_new();

  return str_new("");
}

char *record_read_end (void) {
  if (arr_size(fields) == 0) {
    return str_new("Record without fields");
  }
  return str_new("");
}

char *record_field (char *line) {
  if (str_starts(line, "##") && doc_null) {
    char *l = str_trim(str_right(line, 2));
    doc_null = str_cat(doc_null, "\1", l, NULL);
    return "";
  }

  if (*line == '#') {
    char *l = str_right(line, 1);
    if (*l == '#') l = str_right(l, 1);
    l = str_trim(l);
    doc_null = l;
    return "";
  }

  // Arr[char]
  Arr *ps = str_csplit_trim(line, ':');
  int len = arr_size(ps);

  if (len < 2) {
    return "Expected ':'";
  }

  if (len > 1 && !arr_get(ps, 0)) {
    return "Field name is missing";
  }

  char *name = arr_get(ps, 0);
  if (len == 2) {
    char *tp = arr_get(ps, 1);
    if (!*tp) {
      return "Field type is missing";
    }

    // Arr[char]
    Arr *ps2 = str_csplit_trim(tp, '<');
    int len2 = arr_size(ps2);

    if (len2 < 2) {
      arr_push(fields, field_new(name, tp, NULL));
      return "";
    }

    char *base_tp = arr_get(ps2, 0);
    if (!base_tp) {
      return "Type before '<' is missing";
    }

    if (len2 > 2) {
      return "More than one '<' found";
    }

    if (!str_ends(tp, ">")) {
      return "'>' is missing";
    }

    char *generic = arr_get(ps2, 1);
    generic = str_trim(str_left(generic, strlen(generic) - 1));
    if (!*generic) {
      return "Empty '< >' found";
    }

    if (!is_map(base_tp) && !is_array(base_tp)) {
      return str_f("Type '%s' does not allow generic", base_tp);
    }

    arr_push(fields, field_new(name, base_tp, generic));

    return "";
  }

  return "More than one ':' found";
}

char *record_code (void) {
  int nfields = arr_size(fields);
  // Arr[char]
  Arr *fnames = arr_new();
  Buf *bf = buf_new();

  // -------------------------------------------------------------- CONSTRUCTOR

  // --------------------------------------------------- DOC
  buf_add(bf, "  /**\n");
  EACH(fields, struct Field, f)
    arr_push(fnames, f->name);

    buf_add(bf, "   * @param ");
    buf_add(bf, field_type(f));
    buf_cadd(bf, ' ');
    buf_add(bf, f->name);
    if (f->doc_null) {
      buf_cadd(bf, ' ');
      buf_add(bf, str_replace(f->doc_null, "\1", "\n   *        "));
    }
    buf_cadd(bf, '\n');
  _EACH
  buf_add(bf, "   */\n");
  // -------------------------------------------------- HEAD
  buf_add(bf, "  constructor (");
  if (nfields > 4) {
    buf_add(bf, "\n    ");
  }
  buf_add(bf, str_join(fnames, nfields < 5 ? ", " : ",\n    "));
  if (nfields > 4) {
    buf_add(bf, "\n  ");
  }
  buf_add(bf, ") {\n");
  // -------------------------------------------------- BODY
  EACH(fields, struct Field, f)
    buf_add(bf, str_cat(
      "\n    /**\n",
      "     * @private\n",
      "     * @type ",
      field_type(f),
      "\n     */\n    this._",
      f->name,
      " = ",
      f->name,
      ";\n",
      NULL
    ));
  _EACH
  buf_add(bf, "\n  }\n");

  // ---------------------------------------------------------------- GET / SET

  EACH(fields, struct Field, f)
    char *fn = field_type(f);
    buf_add(bf, str_cat(
      "\n  /**  @return ",
      fn,
      " */\n  get ",
      f->name,
      " () {\n    return this._",
      f->name,
      ";\n  }\n",
      NULL
    ));

    if (f->writable) {
      char *name = str_new(f->name);
      *name = toupper(*name);
      buf_add(bf, str_cat(
        "\n  /**\n",
        "   * @param ",
        fn,
        " value\n   * @return void\n   */\n  set",
        name,
        " (value) {\n    this._",
        f->name,
        " = value;\n  }\n",
        NULL
      ));
    }
  _EACH

  // --------------------------------------------------------------------- JSON

  // ---------------------------------------------------- TO

  if (toJs) {
    buf_add(bf, "\n  /** @return {!Array<?>} */\n  toJs () {\n    return [");
    // Arr[char]
    Arr *fs = arr_new();
    EACH(fields, struct Field, f)
      char *fse = "\n      ";
      if (is_simple_type(f->tp1)) {
        fse = str_cat(fse, "this._", f->name, NULL);
      } else if (f->tp2_null) {
        if (*f->tp1 != '!') {
          fse = str_cat(fse, "this._", f->name, " ? ", NULL);
        }

        if (is_array(f->tp1)) {
          fse = str_cat(fse, "this._", f->name, NULL);
          if (!is_simple_type(f->tp2_null)){
            if (*f->tp2_null == '!') {
              fse = str_cat(fse, ".map(e => e.toJs())", NULL);
            } else {
              fse = str_cat(fse, ".map(e => e ? e.toJs() : null)", NULL);
            }
          }
        } else {
          fse = str_cat(fse, "this._", f->name, ".entries()", NULL);
          if (!is_simple_type(f->tp2_null)){
            if (*f->tp2_null == '!') {
              fse = str_cat(
                fse,
                ".map(kv =>\n"
                "        [kv[0], kv[1].toJs()]\n"
                "      )",
                NULL
              );
            } else {
              fse = str_cat(
                fse,
                ".map(kv =>\n"
                "        [kv[0], kv[1] ? kv[1].toJs() : null]\n"
                "      )",
                NULL
              );
            }
          }
        }

        if (*f->tp1 != '!') {
          fse = str_cat(fse, " : null", NULL);
        }

      } else {
        fse = str_cat(fse, "this._", f->name, NULL);
        if (*f->tp1 == '!') {
          fse = str_cat(fse, ".toJs()", NULL);
        } else {
          fse = str_cat(
            fse,
            " ? ",
            "this._",
            f->name,
            ".toJs()",
            " : null",
            NULL
          );
        }
      }
      arr_push(fs, fse);
    _EACH
    buf_add(bf, str_cjoin(fs, ','));
    buf_add(bf, "\n    ];\n  }\n");
  }

  // -------------------------------------------------- FROM

  if (fromJs) {
    buf_add(bf, str_cat(
      "\n  /**\n"
      "   * @param {!Array<?>} serial\n"
      "   * @return {!",
      cname,
      "}\n"
      "   */\n"
      "  static fromJs (serial) {\n"
      "    return new ",
      cname,
      "(",
      NULL
    ));

    // Arr[char]
    Arr *fs = arr_new();
    EACH_IX(fields, struct Field, f, i)
      char *ix = str_f("%d", i);
      char *fse = "\n      ";
      if (is_simple_type(f->tp1)) {
        fse = str_cat(fse, "serial[", ix, "]", NULL);
      } else {
        char *tp = f->tp1;
        if (*tp != '!') {
          fse = str_cat(fse, "serial[", ix, "] ? ", NULL);
        } else {
          tp = str_right(tp, 1);
        }

        if (f->tp2_null) {
          if (is_array(f->tp1)) {
            fse = str_cat(fse, "serial[", ix, "]", NULL);
            if (!is_simple_type(f->tp2_null)) {
              char *tp2 = f->tp2_null;
              fse = str_cat(fse, ".map(e =>\n        ", NULL);
              if (*f->tp2_null != '!') {
                fse = str_cat(fse, "e ? ", NULL);
              } else {
                tp2 = str_right(tp2, 1);
              }

              fse = str_cat(fse, tp2, ".fromJs(e)", NULL);

              if (*f->tp2_null != '!') {
                fse = str_cat(fse, " : null", NULL);
              }

              fse = str_cat(fse, "\n      )", NULL);
            }
          } else if (is_map(f->tp1)) {
            fse = str_cat(fse, "new Map(serial[", ix, "]", NULL);
            if (!is_simple_type(f->tp2_null)) {
              char *tp2 = str_new(f->tp2_null);
              fse = str_cat(fse, ".map(kv =>\n        [kv[0], ", NULL);
              if (*f->tp2_null != '!') {
                fse = str_cat(fse, "kv[1] ? ", NULL);
              } else {
                tp2 = str_right(tp2, 1);
              }

              fse = str_cat(fse, tp2, ".fromJs(kv[1])", NULL);
              if (*f->tp2_null != '!') {
                fse = str_cat(fse, " : null", NULL);
              }

              fse = str_cat(fse, "]\n      )", NULL);
            }
            fse = str_cat(fse, ")", NULL);
          } else {
            fse = str_cat(fse, "new Map(", "serial[", ix, "]", ")", NULL);
          }
        } else {
          fse = str_cat(fse, tp, ".fromJs(serial[", ix, "])", NULL);
        }

        if (*f->tp1 != '!') {
          fse = str_cat(fse, " : null", NULL);
        }
      }
      arr_push(fs, fse);
    _EACH

    buf_add(bf, str_cjoin(fs, ','));
    buf_add(bf, "\n    );\n  }\n");
  }

  return buf_to_str(bf);
}
