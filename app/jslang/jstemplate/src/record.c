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


static int is_simple_type (const char *tp) {
  char *simple_types = " boolean boolean= number number= string string= ";
  char *t = str_f_new(" %s ", tp);
  int r = str_index(simple_types, t) != -1;
  free(t);
  return r;
}

static int is_array (const char *tp) {
  return *tp == '!' ? str_eq(tp, "!Array") : str_eq(tp, "Array");
}

static int is_map (const char *tp) {
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
  struct Field *this = malloc(sizeof(struct Field));
  this->doc_null = doc_null ? str_new(doc_null) : NULL;
  doc_null = NULL;
  this->writable = *name == '+';
  char *nm = str_new(name);
  if (this->writable) {
    char *tmp = nm;
    nm = str_right_new(tmp, 1);
    free(tmp);
  }
  this->name = nm;
  this->tp1 = str_new(tp1);
  this->tp2_null = tp2_null ? str_new(tp2_null) : NULL;
  return this;
}

static void field_free(struct Field *this) {
  free(this->doc_null);
  free(this->name);
  free(this->tp1);
  free(this->tp2_null);
  free(this);
}

static char *field_type_new(struct Field *this) {
    char *tmp = NULL;
    char *r = str_cat_new("{", this->tp1, NULL);
    if (this->tp2_null) {
      tmp = r;
      r = str_cat_new(tmp, "<", NULL);
      free(tmp);
      if (is_map(this->tp1)) {
        tmp = r;
        r = str_cat_new(tmp, "string, ", NULL);
        free(tmp);
      }
      tmp = r;
      r = str_cat_new(tmp, this->tp2_null, ">", NULL);
      free(tmp);
    }
    tmp = r;
    r = str_cat_new(tmp, "}", NULL);
    free(tmp);
    return r;
}

char *record_init_new(char *line) {
  char *l = str_right_new(line, 4);
  // Arr[char]
  Arr *ps = str_csplit_trim_new(l, ':');
  int pslen = arr_size(ps);

  if (pslen > 2) {
    free(l);
    arr_free(ps);
    return str_new("Unexpected second ':'");
  }

  if (pslen == 0 || !*((char *)arr_get(ps, 0))) {
    free(l);
    free(ps);
    return str_new("Class name is mising");
  }

  toJs = 0;
  fromJs = 0;

  free(cname);
  cname = str_new(arr_get(ps, 0));

  if (pslen == 2) {
    free(l);
    l = arr_get(ps, 1);
    if (!*l) {
      free(l);
      free(ps);
      return str_new("Expected 'to|from|serial'");
    } else if (str_eq(l, "to")) {
      toJs = 1;
    } else if (str_eq(l, "from")) {
      fromJs = 1;
    } else if (str_eq(l, "serial")) {
      toJs = 1;
      fromJs = 1;
    } else {
      char *r = str_f_new("Expected 'to|from|serial'. Found '%s'", l);
      free (l);
      free(ps);
      return r;
    }
  }

  free(doc_null);
  doc_null = NULL;
  arr_free(fields);
  fields = arr_new((FPROC)field_free);

  free(l);
  free(ps);
  return str_new("");
}

char *record_read_end_new (void) {
  if (arr_size(fields) == 0) {
    return str_new("Record without fields");
  }
  return str_new("");
}

char *record_field_new (char *line) {
  if (str_starts(line, "##") && doc_null) {
    char *tmp = str_right_new(line, 2);
    char *l = str_trim_new(tmp);
    free(tmp);
    tmp = doc_null;
    doc_null = str_cat_new(tmp, "\1", l, NULL);
    free(tmp);
    free(l);
    return "";
  }

  if (*line == '#') {
    char *l = str_new(line);
    char *tmp = l;
    l = str_right_new(tmp, 1);
    free(tmp);
    if (*l == '#') {
      tmp = l;
      l = str_right_new(tmp, 1);
      free(tmp);
    }
    tmp = l;
    l = str_trim_new(tmp);
    free(tmp);
    free(doc_null);
    doc_null = l;
    return "";
  }

  // Arr[char]
  Arr *ps = str_csplit_trim_new(line, ':');
  int len = arr_size(ps);

  if (len < 2) {
    arr_free(ps);
    return str_new("Expected ':'");
  }

  if (len > 1 && !arr_get(ps, 0)) {
    arr_free(ps);
    return str_new("Field name is missing");
  }

  char *name = arr_get(ps, 0);
  if (len == 2) {
    char *tp = arr_get(ps, 1);
    if (!*tp) {
      arr_free(ps);
      return str_new("Field type is missing");
    }

    // Arr[char]
    Arr *ps2 = str_csplit_trim_new(tp, '<');
    int len2 = arr_size(ps2);

    if (len2 < 2) {
      arr_push(fields, field_new(name, tp, NULL));
      arr_free(ps);
      arr_free(ps2);
      return str_new("");
    }

    char *base_tp = arr_get(ps2, 0);
    if (!base_tp) {
      arr_free(ps);
      arr_free(ps2);
      return str_new("Type before '<' is missing");
    }

    if (len2 > 2) {
      arr_free(ps);
      arr_free(ps2);
      return str_new("More than one '<' found");
    }

    if (!str_ends(tp, ">")) {
      arr_free(ps);
      arr_free(ps2);
      return str_new("'>' is missing");
    }

    char *generic = str_new(arr_get(ps2, 1));
    char *tmp = generic;
    generic = str_left_new(tmp, strlen(generic) - 1);
    free(tmp);
    tmp = generic;
    generic = str_trim_new(tmp);
    free(tmp);
    if (!*generic) {
      free(generic);
      arr_free(ps);
      arr_free(ps2);
      return str_new("Empty '< >' found");
    }

    if (!is_map(base_tp) && !is_array(base_tp)) {
      free(generic);
      arr_free(ps);
      arr_free(ps2);
      return str_f_new("Type '%s' does not allow generic", base_tp);
    }

    arr_push(fields, field_new(name, base_tp, generic));

    free(generic);
    arr_free(ps);
    arr_free(ps2);
    return str_new("");
  }

  arr_free(ps);
  return str_new("More than one ':' found");
}

char *record_code_new (void) {
  int nfields = arr_size(fields);
  // Varr[char]
  Varr *fnames = varr_new();
  char *r = str_new("");
  char *tmp = NULL;

  // -------------------------------------------------------------- CONSTRUCTOR

  // --------------------------------------------------- DOC
  tmp = r;
  r = str_cat_new(tmp, "  /**\n");
  free(tmp);
  EACH(fields, struct Field, f)
    varr_push(fnames, f->name);

    char *nf = field_type_new(f);
    tmp = r;
    r = str_cat_new(tmp, "   * @param ", nf, " ", f->name, NULL);
    free(tmp);
    free(nf);
    if (f->doc_null) {
      char *d = str_replace_new(f->doc_null, "\1", "\n   *        ");
      tmp = r;
      r = str_cat_new(tmp, " ", d, NULL);
      free(tmp);
      free(d);
    }
    tmp = r;
    r = str_cat_new(tmp, "\n", NULL);
    free(tmp);
  _EACH
  tmp = r;
  r = str_cat_new(tmp, "   */\n", NULL);
  free(tmp);
  // -------------------------------------------------- HEAD
  tmp = r;
  r = str_cat_new(tmp, "  constructor (", NULL);
  free(tmp);
  if (nfields > 4) {
    tmp = r;
    r = str_cat_new(tmp, "\n    ", NULL);
    free(tmp);
  }
  tmp = r;
  char *tmp2 = str_join_new(
    (Arr*)fnames,
    nfields < 5 ? ", " : ",\n    "
  );
  r = str_cat_new(tmp, tmp2, NULL);
  free(tmp);
  free(tmp2);
  if (nfields > 4) {
    tmp = r;
    r = str_cat_new(tmp, "\n  ", NULL);
    free(tmp);
  }
  tmp = r;
  r = str_cat_new(tmp, ") {\n", NULL);
  free(tmp);
  // -------------------------------------------------- BODY
  EACH(fields, struct Field, f)
    char *fn = field_type_new(f);
    tmp = r;
    r = str_cat_new(
      tmp,
      "\n    /**\n",
      "     * @private\n",
      "     * @type ",
      fn,
      "\n     */\n    this._",
      f->name,
      " = ",
      f->name,
      ";\n",
      NULL
    );
    free(tmp);
    free(fn);
  _EACH
  tmp = r;
  r = str_cat_new(tmp, "\n  }\n");
  free(tmp);

  // ---------------------------------------------------------------- GET / SET

  EACH(fields, struct Field, f)
    char *fn = field_type_new(f);
    tmp = r;
    r = str_cat_new(
      tmp,
      "\n  /**  @return ",
      fn,
      " */\n  get ",
      f->name,
      " () {\n    return this._",
      f->name,
      ";\n  }\n",
      NULL
    );
    free(tmp);

    if (f->writable) {
      char *name = str_new(f->name);
      *name = toupper(*name);
      tmp = r;
      str_cat_new(
        tmp,
        "\n  /**\n",
        "   * @param ",
        fn,
        " value\n   * @return void\n   */\n  set",
        name,
        " (value) {\n    this._",
        f->name,
        " = value;\n  }\n",
        NULL
      );
      free(name);
      free(tmp);
    }
    free(fn);
  _EACH

  // --------------------------------------------------------------------- JSON

  // ---------------------------------------------------- TO

  if (toJs) {
    tmp = r;
    r = str_cat_new(
      tmp,
      "\n  /** @return {!Array<?>} */\n  toJs () {\n    return [",
      NULL
    );
    free(tmp);
    // Arr[char]
    Arr *fs = arr_new(free);
    EACH(fields, struct Field, f)
      char *fse = str_new("\n      ");
      if (is_simple_type(f->tp1)) {
        tmp = fse;
        fse = str_cat_new(tmp, "this._", f->name, NULL);
        free(tmp);
      } else if (f->tp2_null) {
        if (*f->tp1 != '!') {
          tmp = fse;
          fse = str_cat_new(tmp, "this._", f->name, " ? ", NULL);
          free(tmp);
        }

        if (is_array(f->tp1)) {
          tmp = fse;
          fse = str_cat_new(tmp, "this._", f->name, NULL);
          free(tmp);
          if (!is_simple_type(f->tp2_null)){
            tmp = fse;
            if (*f->tp2_null == '!') {
              fse = str_cat_new(tmp, ".map(e => e.toJs())", NULL);
            } else {
              fse = str_cat_new(tmp, ".map(e => e ? e.toJs() : null)", NULL);
            }
            free(tmp);
          }
        } else {
          tmp = fse;
          fse = str_cat_new(tmp, "this._", f->name, ".entries()", NULL);
          free(tmp);
          if (!is_simple_type(f->tp2_null)){
            tmp = fse;
            if (*f->tp2_null == '!') {
              fse = str_cat_new(
                tmp,
                ".map(kv =>\n"
                "        [kv[0], kv[1].toJs()]\n"
                "      )",
                NULL
              );
            } else {
              fse = str_cat_new(
                tmp,
                ".map(kv =>\n"
                "        [kv[0], kv[1] ? kv[1].toJs() : null]\n"
                "      )",
                NULL
              );
            }
            free(tmp);
          }
        }

        if (*f->tp1 != '!') {
          tmp = fse;
          fse = str_cat_new(tmp, " : null", NULL);
          free(tmp);
        }

      } else {
        tmp = fse;
        fse = str_cat_new(tmp, "this._", f->name, NULL);
        free(tmp);
        tmp = fse;
        if (*f->tp1 == '!') {
          fse = str_cat_new(tmp, ".toJs()", NULL);
        } else {
          fse = str_cat_new(
            tmp,
            " ? ",
            "this._",
            f->name,
            ".toJs()",
            " : null",
            NULL
          );
        }
        free(tmp);
      }
      arr_push(fs, fse);
    _EACH
    tmp = r;
    char *tmp2 = str_cjoin_new(fs, ',');
    r = str_cat_new(tmp, tmp2, "\n    ];\n  }\n", NULL);
    free(tmp);
    free(tmp2);
    arr_free(fs);
  }

  // -------------------------------------------------- FROM

  if (fromJs) {
    tmp = r;
    r = str_cat_new(
      tmp,
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
    );
    free(tmp);

    // Arr[char]
    Arr *fs = arr_new(free);
    EACH_IX(fields, struct Field, f, i)
      char *ix = str_f_new("%d", i);
      char *fse = str_new("\n      ");
      if (is_simple_type(f->tp1)) {
        tmp = fse;
        fse = str_cat_new(tmp, "serial[", ix, "]", NULL);
        free(tmp);
      } else {
        char *tp = str_new(f->tp1);
        if (*tp != '!') {
          tmp = fse;
          fse = str_cat_new(tmp, "serial[", ix, "] ? ", NULL);
          free(tmp);
        } else {
          tmp = tp;
          tp = str_right_new(tmp, 1);
          free(tmp);
        }

        if (f->tp2_null) {
          if (is_array(f->tp1)) {
            tmp = fse;
            fse = str_cat_new(tmp, "serial[", ix, "]", NULL);
            free(tmp);
            if (!is_simple_type(f->tp2_null)) {
              char *tp2 = str_new(f->tp2_null);
              tmp = fse;
              fse = str_cat_new(tmp, ".map(e =>\n        ", NULL);
              free(tmp);
              if (*f->tp2_null != '!') {
                tmp = fse;
                fse = str_cat_new(tmp, "e ? ", NULL);
                free(tmp);
              } else {
                tmp = tp2;
                tp2 = str_right_new(tmp, 1);
                free(tmp);
              }

              tmp = fse;
              fse = str_cat_new(tmp, tp2, ".fromJs(e)", NULL);
              free(tmp);

              if (*f->tp2_null != '!') {
                tmp = fse;
                tmp = str_cat_new(tmp, " : null", NULL);
                free(tmp);
              }

              tmp = fse;
              fse = str_cat_new(tmp, "\n      )", NULL);
              free(tmp);
              free(tp2);
            }
          } else if (is_map(f->tp1)) {
            tmp = fse;
            fse = str_cat_new(tmp, "new Map(serial[", ix, "]", NULL);
            free(tmp);
            if (!is_simple_type(f->tp2_null)) {
              char *tp2 = str_new(f->tp2_null);
              tmp = fse;
              fse = str_cat_new(tmp, ".map(kv =>\n        [kv[0], ", NULL);
              free(tmp);
              if (*f->tp2_null != '!') {
                tmp = fse;
                fse = str_cat_new(tmp, "kv[1] ? ", NULL);
                free(tmp);
              } else {
                tmp = tp2;
                tp2 = str_right_new(tmp, 1);
                free(tmp);
              }

              tmp = fse;
              fse = str_cat_new(tmp, tp2, ".fromJs(kv[1])", NULL);
              free(tmp);
              if (*f->tp2_null != '!') {
                tmp = fse;
                fse = str_cat_new(tmp, " : null", NULL);
                free(tmp);
              }

              tmp = fse;
              fse = str_cat_new(tmp, "]\n      )", NULL);
              free(tmp);
              free(tp2);
            }
            tmp = fse;
            fse = str_cat_new(tmp, ")", NULL);
            free(tmp);
          } else {
            tmp = fse;
            fse = str_cat_new(tmp, "new Map(", "serial[", ix, "]", ")", NULL);
            free(tmp);
          }
        } else {
          tmp = fse;
          fse = str_cat_new(tmp, tp, ".fromJs(serial[", ix, "])", NULL);
          free(tmp);
        }

        if (*f->tp1 != '!') {
          tmp = fse;
          fse = str_cat_new(tmp, " : null", NULL);
          free(tmp);
        }
        free(tp);
      }
      arr_push(fs, fse);
      free(ix);
    _EACH
    tmp = r;
    char *tmp2 = str_cjoin_new(fs, ',');
    r = str_cat_new(tmp, tmp2, "\n    );\n  }\n", NULL);
    free(tmp2);
    free(tmp);

    arr_free(fs);
  }

  varr_free(fnames);
  return r;
}

void record_free (void) {
  free(cname);
  free(doc_null);
  arr_free(fields);
}
