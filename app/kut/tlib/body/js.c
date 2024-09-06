static void json_unicode(Buf *bf, char *hexdigits) {
  char hexvalue (char ch) {
    return (ch <= '9') ? ch - '0' : toupper(ch) - 'A' + 10;
  }
  unsigned int codepoint =
    hexvalue(*hexdigits) * 4096 +
    hexvalue(*(hexdigits + 1)) * 256 +
    hexvalue(*(hexdigits + 2)) * 16 +
    hexvalue(*(hexdigits + 3));
  if (codepoint < 0x80) {
    buf_cadd(bf, (codepoint>>0  & 0x7F) | 0x00);
    return;
  }
  if (codepoint < 0x0800) {
    buf_cadd(bf, (codepoint>>6  & 0x1F) | 0xC0);
    buf_cadd(bf, (codepoint>>0  & 0x3F) | 0x80);
    return;
  }
  buf_cadd(bf, (codepoint>>12 & 0x0F) | 0xE0);
  buf_cadd(bf, (codepoint>>6  & 0x3F) | 0x80);
  buf_cadd(bf, (codepoint>>0  & 0x3F) | 0x80);
}
static char *json_blanks(char *json) {
  while (*json && ((unsigned char)*json) <= ' ') {
    ++json;
  }
  return json;
}
static char *json_snull(char *json) {
  return json_blanks(json + 4);
}
static char *json_sbool(char *json) {
  if (*json == 't') {
    return json_blanks(json + 4);
  }
  return json_blanks(json + 5);
}
static char *json_snumber(char *j) {
  while (
    *j && *j != '}' && *j != ']' && *j != ',' && ((unsigned char) *j) > ' '
  ) {
    ++j;
  }
  return json_blanks(j);
}
static char *json_sstring(char *j) {
  ++j;
  while (*j && *j != '"') {
    if (*j++ == '\\') {
      if (*j) {
        ++j;
      }
    }
  }
  if (*j) {
    ++j;
  }
  return json_blanks(j);
}
static char *json_selement(char *j);
static char *json_sarray(char *j) {
  ++j;
  j = json_blanks(j);
  while (*j && *j != ']') {
    j = json_selement(j);
    if (*j == ',') {
      ++j;
      j = json_blanks(j);
    }
  }
  if (*j) {
    ++j;
  }
  return json_blanks(j);
}
static char *json_sobject(char *j) {
  ++j;
  j = json_blanks(j);
  while (*j && *j != '}') {
    j = json_sstring(j);
    ++j;
    j = json_selement(j);
    if (*j == ',') {
      ++j;
      j = json_blanks(j);
    }
  }
  if (*j) {
    ++j;
  }
  return json_blanks(j);
}
static char *json_selement(char *j) {
  j = json_blanks(j);
  switch (*j) {
  case 'n': return json_snull(j);
  case 't':
  case 'f': return json_sbool(j);
  case '"': return json_sstring(j);
  case '[': return json_sarray(j);
  case '{': return json_sobject(j);
  case '-': return json_snumber(j);
  default :
    if (*j >= '0' && *j <= '9') {
      return json_snumber(j);
    } else {
      if (*j) {
        ++j;
      }
      return j;
    }
  }
}
static int json_rend(char *json) {
  while (*json && ((unsigned char)*json) <= ' ') {
    ++json;
  }
  return *json ? 0 : 1;
}
static char *json_error(char *json) {
  Arrs *runes = str_to_runes(json);
  return tarr_sizeS(runes) > 70
    ? str_f("%s...", tarr_join(tarr_takeS(runes, 67), ""))
    : json
  ;
}
static int js_is_null (char *json) {
  char *j = json_blanks(json);
  if (memcmp(j, "null", 4)) {
    return 0;
  }
  return json_rend(j + 4);
}

static int js_rb (char *json) {
  char *jsons = json_blanks(json);
  int r;
  char *j = jsons;
  if (memcmp(j, "true", 4)) {
    if (memcmp(j, "false", 5))
      texc_expect("js.rb",
        "Bad JSON string", "Boolean value", json_error(json)
      );
    r = 0;
    j += 5;
  } else {
    r = 1;
    j += 4;
  }
  if (!json_rend(j))
    texc_expect("js.rb",
      "Bad JSON string", "Boolean value (spare characters)", json_error(json)
    );
  return r;
}
static int64_t js_ri (char *json) {
  char *jsons = json_blanks(json);
  char *j = jsons;
  if (*j != '-' && (*j < '0' || *j > '9'))
    texc_expect("js.ri",
      "Bad JSON string", "Long value", json_error(json)
    );
  Buf *bf = buf_new();
  while (
    *j && *j != '}' && *j != ']' && *j != ',' && ((unsigned char)*j) > ' '
  ) {
    ++j;
  }
  buf_add_buf(bf, jsons, j - jsons);
  char *n = buf_str(bf);
  if (!json_rend(j))
    texc_expect("js.ri",
      "Bad JSON string", "Long value (spare characters)", json_error(json)
    );
  char *tail;
  errno = 0;
  int64_t r = strtol(n, &tail, 10);
  if (errno)
    texc_throw("js.ri",
      str_f("Bad JSON string: Long overflow in %s", json_error(json))
    );
  if (*tail)
    texc_expect("js.ri",
      "Bad JSON string", "Long value", json_error(json)
    );
  return r;
}
static double js_rf (char *json) {
  struct lconv *lc = localeconv();
  char *jsons = json_blanks(json);
  char *j = jsons;
  if (*j != '-' && (*j < '0' || *j > '9'))
    texc_expect("js.rf",
     "Bad JSON string", "Float value", json_error(json)
    );
  Buf *bf = buf_new();
  while (
    *j && *j != '}' && *j != ']' && *j != ',' && ((unsigned char)*j) > ' '
  ) {
    ++j;
  }
  buf_add_buf(bf, jsons, j - jsons);
  char *n = buf_str(bf);
  int ix = str_cindex(n, '.');
  if (ix != -1) {
    n[ix] = *lc->decimal_point;
  }
  if (!json_rend(j))
    texc_expect("js.rf",
      "Bad JSON string", "Float value (spare characters)", json_error(json)
    );
  errno = 0;
  char *tail;
  double r = strtod(n, &tail);
  if (errno)
    texc_throw("js.rf",
      str_f("Bad JSON string: Float overflow in %s", json_error(json))
    );
  if (*tail)
    texc_expect("js.rf",
     "Bad JSON string", "Float value", json_error(json)
    );
  return r;
}
static char *js_rs (char *j) {
  int is_hex (char ch) {
    return (ch >= '0' && ch <= '9') ||
      (ch >= 'a' && ch <= 'f') ||
      (ch >= 'A' && ch <= 'F');
  }
  char *json = json_blanks(j);
  if (*json != '"')
    texc_expect("js.rs",
      "Bad JSON string", "String value (not begin with '\"')", json_error(j)
    );
  ++json;
  Buf *bf = buf_new();
  while (*json && *json != '"') {
    if (*json == '\\') {
      ++json;
      switch (*json) {
        case '"' :
        case '\\' :
        case '/' :
          buf_cadd(bf, *json);
          break;
        case 'b' :
          buf_cadd(bf, '\b');
          break;
        case 'f' :
          buf_cadd(bf, '\f');
          break;
        case 'n' :
          buf_cadd(bf, '\n');
          break;
        case 'r' :
          buf_cadd(bf, '\r');
          break;
        case 't' :
          buf_cadd(bf, '\t');
          break;
        case 'u' : {
          ++json;
          int c = 5;
          while (--c) {
            if (!is_hex(*json++))
              texc_expect("js.rs",
                "Bad JSON string", "String value (Bad unicode)", json_error(j)
              );
          }
          json_unicode(bf, json - 4);
          continue;
        }
        default:
          texc_expect("js.rs",
            "Bad JSON string", "String value (Bad escape sequence)", json_error(j)
          );
      }
      ++json;
    } else {
      buf_cadd(bf, *json++);
    }
  }
  if (!*json)
    texc_expect("js.rs",
      "Bad JSON string", "String value (not end with '\"')", json_error(j)
    );
  if (!json_rend(json + 1))
    texc_expect("js.rs",
      "Bad JSON string", "String value (spare characters)", json_error(j)
    );
  char *r = str_new(buf_str(bf));
  return r;
}
static Arrs *js_ra (char *j) {
  char *json = json_blanks(j);
  if (*json != '[')
    texc_expect("js.ra",
      "Bad JSON string", "Array value (not begin with '[')", json_error(j)
    );
  ++json;
  Arrs *a = tarr_new_s();
  while (*json && *json != ']') {
    char *tmp = json;
    json = json_selement(json);
    Buf *bf = buf_new();
    buf_add_buf(bf, tmp, json - tmp);
    tarr_pushS(a, str_new(buf_str(bf)));
    if (*json == ',') {
      ++json;
    } else if (*json && *json != ']')
      texc_expect("js.ra",
        "Bad JSON string", "Array value (comma missing)", json_error(j)
      );
  }
  if (!*json)
    texc_expect("js.ra",
      "Bad JSON string", "Array value (not end with ']')", json_error(j)
    );
  if (!json_rend(json + 1))
    texc_expect("js.ra",
      "Bad JSON string", "Array value (spare characters)", json_error(j)
    );
  return a;
}

static Arrs *js_ro (char *j) {
  char *json = json_blanks(j);
  if (*json != '{')
    texc_expect("js.ro",
      "Bad JSON string", "Object value (not begin with '{')", json_error(j)
    );
  json = json_blanks(json + 1);
  Arrs *m = tarr_new_s();
  while (*json && *json != '}') {
    if (*json != '"')
      texc_expect("js.ro",
        "Bad JSON string", "Object value (bad key)", json_error(j)
      );
    char *tmp = json;
    json = json_sstring(json);
    Buf *kbf = buf_new();
    buf_add_buf(kbf, tmp, json - tmp);
    if (*json != ':')
      texc_expect("js.ro",
        "Bad JSON string", "Object value (':' is missing)", json_error(j)
      );
    ++json;
    tmp = json;
    json = json_selement(json);
    Buf *vbf = buf_new();
    buf_add_buf(vbf, tmp, json - tmp);
    dic_put(m, js_rs(buf_str(kbf)), str_new(buf_str(vbf)));
    if (*json == ',') {
      json = json_blanks(json + 1);
    } else if (*json && *json != '}')
      texc_expect("js.ro",
        "Bad JSON string", "Object value (comma missing)", json_error(j)
      );
  }
  if (!*json)
    texc_expect("js.ro",
      "Bad JSON string", "Object value (not end with '}')", json_error(j)
    );
  if (!json_rend(json + 1))
    texc_expect("js.ro",
      "Bad JSON string", "Object value (spare characters)", json_error(j)
    );
  return m;
}
static char *js_wb(int value) {
  return str_new(value ? "true" : "false");
}
static char *js_wi(int64_t n) {
  return str_f("%ld", n);
}
static char *js_wf(double n) {
  return math_ftos(n);
}
static char *js_wf2(double n, int64_t scale) {
  return math_ftos2(n, scale);
}
static char *js_ws(char *s) {
  Buf *bf = buf_new();
  char tmp;
  buf_cadd(bf, '"');
  while (*s) {
    tmp = *s++;
    switch (tmp) {
    case '"':
      buf_add(bf, "\\\"");
      break;
    case '\\':
      buf_add(bf, "\\\\");
      break;
    case '\b':
      buf_add(bf, "\\b");
      break;
    case '\f':
      buf_add(bf, "\\f");
      break;
    case '\n':
      buf_add(bf, "\\n");
      break;
    case '\r':
      buf_add(bf, "\\r");
      break;
    case '\t':
      buf_add(bf, "\\t");
      break;
    default:
      buf_cadd(bf, tmp);
    }
  }
  buf_cadd(bf, '"');
  char *r = str_new(buf_str(bf));
  return r;
}
static char *js_wa(Arrs *a) {
  Buf *bf = buf_new();
  buf_cadd(bf, '[');
  char **p = tarr_beginS(a);
  char **end = tarr_endS(a);
  if (p < end) {
    buf_add(bf, *p++);
    while (p < end) {
      buf_cadd(bf, ',');
      buf_add(bf, *p++);
    }
  }
  buf_cadd(bf, ']');
  char *r = str_new(buf_str(bf));
  return r;
}
static char *js_wo(Arrs *m) {
  Buf *bf = buf_new();
  buf_cadd(bf, '{');
  char **p = tarr_beginS(m);
  char **end = tarr_endS(m);
  if (p < end) {
    buf_add(bf, js_ws(*p++));
    buf_cadd(bf, ':');
    buf_add(bf, *p++);
    while (p < end) {
      buf_cadd(bf, ',');
      buf_add(bf, js_ws(*p++));
      buf_cadd(bf, ':');
      buf_add(bf, *p++);
    }
  }
  buf_cadd(bf, '}');
  char *r = str_new(buf_str(bf));
  return r;
}
