static int str_cindex (char *str, char ch) {
  int c = 0;
  char *p  = str;
  while (*p) {
    if (*p++ == ch) {
      return c;
    };
    ++c;
  }
  return -1;
}
static int str_cindex_from (char *str, char ch, int from) {
  if (from >= strlen(str)) {
    return -1;
  }
  int c = from;
  char *p  = str + from;
  while (*p) {
    if (*p++ == ch) {
      return c;
    };
    ++c;
  }
  return -1;
}
static char *str_creplace (char *s, char old, char new) {
  char *r = str_new(s);
  char *p = r;
  while (*p) {
    if (*p == old)
      *p = new;
    ++p;
  }
  return r;
}
static Arrs *str_csplit (char *s, char sep) {
  Arrs *r = tarr_new_s();
  int i = str_cindex(s, sep);
  while (i != -1) {
    char *sub = ATOMIC(i + 1);
    sub[i] = 0;
    memcpy(sub, s, i);
    tarr_pushS(r, sub);
    s = s + i + 1;
    i = str_cindex(s, sep);
  }
  tarr_pushS(r, str_new(s));
  return r;
}
static int str_ends (char *str, char  *substr) {
  int slen = strlen(str);
  int sublen = strlen(substr);
  return slen >= sublen && !memcmp(str + slen - sublen, substr, sublen);
}
static char *str_f (char *format, ...) {
  va_list args;
  va_start(args, format);
  char *r = str_vf(format, args);
  va_end(args);
  return r;
}
static char *str_fmt (char *format, Arrs *values) {
  int len = strlen(format);
  int ix = 0;
  int p = 0;
  Buf *r = buf_new();
  for (;;) {
    int i = str_cindex_from(format, '%', p);
    if (i == -1) break;
    buf_add(r, str_sub(format, p, i));
    ++i;
    if (i == len)
    texc_throw("str.fmt", str_f("Bad '%' sequence at end in %s", format));
    char ch = format[i];
    if (ch == '%') {
      buf_cadd(r, ch);
      p = i + 1;
      continue;
    }
    if (ch == 's' || ch == 'v') {
      if (tarr_sizeS(values) <= ix)
        texc_throw("str.fmt",
          str_f("There are values missing in '%s'", format)
        );
      switch (ch) {
      case 's':
      case 'v':
        buf_add(r, tarr_getS(values, ix));
        break;
      default:
        texc_throw("str.fmt", str_f(
          "Bad '%' sequence in %s (only %%s and %%v are valid)", format
        ));
      }
      ++ix;
      p = i + 1;
      continue;
    }
    texc_throw("str.fmt", str_f("Bad '%' sequence in %s", format));
  }
  if (ix < tarr_sizeS(values))
    texc_throw("str.fmt",
      str_f("There are too many values in %s", built_to_strS(values))
    );
  buf_add(r, str_right(format, p));
  return str_new(buf_str(r));
}
static char *str_from_escape (char *s) {
  int len = strlen(s);
  if (len < 2) {
    return str_new(s);
  }
  ++s;
  Buf *bf = buf_new();
  while (*s) {
    char ch = *s++;
    if (ch != '\\') {
      buf_cadd(bf, ch);
    } else {
      buf_cadd(bf, *s++);
    }
  }
  return str_left(buf_str(bf), buf_len(bf) - 1);
}
static char *str_from_iso (char *s) {
  Buf *bf = buf_new();
  unsigned char ch = *s++;
  while (ch) {
    if(ch < 0x80) {
      buf_cadd(bf, ch);
    } else {
      buf_cadd(bf, 0xc0 | (ch & 0xc0) >> 6);
      buf_cadd(bf, 0x80 | (ch & 0x3f));
    }
    ch = *s++;
  }
  return str_new(buf_str(bf));
}
static char *str_from_unicode (unsigned *u) {
  Buf *bf = buf_new();
  while (*u) {
    unsigned code_point = *u++;
    if (code_point < 0x80) {
      buf_cadd(bf, (unsigned char)code_point);
    } else if (code_point <= 0x7FF) {
      buf_cadd(bf, (unsigned char)(code_point >> 6) + 0xC0);
      buf_cadd(bf, (unsigned char)(code_point & 0x3F) + 0x80);
    } else if (code_point <= 0xFFFF) {
      buf_cadd(bf, (unsigned char)(code_point >> 12) + 0xE0);
      buf_cadd(bf, (unsigned char)((code_point >> 6) & 0x3F) + 0x80);
      buf_cadd(bf, (unsigned char)(code_point & 0x3F) + 0x80);
    } else if (code_point <= 0x10FFFF) {
      buf_cadd(bf, (unsigned char)(code_point >> 18) + 0xF0);
      buf_cadd(bf, (unsigned char)((code_point >> 12) & 0x3F) + 0x80);
      buf_cadd(bf, (unsigned char)((code_point >> 6) & 0x3F) + 0x80);
      buf_cadd(bf, (unsigned char)(code_point & 0x3F) + 0x80);
    } else {
      buf_add(bf, "�");
    }
  }
  return str_new(buf_str(bf));
}
static char *str_from_utf16 (Arri *codepoints) {
  int sz = tarr_sizeI(codepoints);
  unsigned *u = ATOMIC(sz * sizeof(unsigned) + 1);
  unsigned *p = u;
  int64_t *begin = tarr_beginI(codepoints);
  int64_t *end = tarr_endI(codepoints);
  while(begin < end) {
    *p++ = (unsigned) *begin++;
  }
  *p = 0;
  return str_from_unicode(u);
}
static char *str_get (char *str, int64_t ix) {
  texc_check_range("str_get", 0, strlen(str) - 1, ix);
  return str_new_c(str[ix]);
}
static int str_index (char *str, char *substr) {
  if (!*substr) {
    return 0;
  }
  int c = 0;
  int limit = strlen(str) - strlen(substr);
  char *p  = str;
  for (;;) {
    if (c > limit) {
      break;
    }
    if (str_starts(p++, substr)) {
      return c;
    };
    ++c;
  }
  return -1;
}
static int str_index_from (char *str, char *substr, int64_t from) {
  if (from >= strlen(str)) {
    return -1;
  }
  if (!*substr) {
    return from;
  }
  int c = from;
  int limit = strlen(str) - strlen(substr);
  char *p  = str + from;
  for (;;) {
    if (c > limit) {
      break;
    }
    if (str_starts(p++, substr)) {
      return c;
    };
    ++c;
  }
  return -1;
}
static int str_last_cindex (char *str, char ch) {
  int r = -1;
  int c = 0;
  char *p  = str;
  while (*p) {
    if (*p++ == ch) {
      r = c;
    };
    ++c;
  }
  return r;
}
static int str_last_index (char *str, char *substr) {
  int r = -1;
  if (!*substr) {
    return strlen(str);
  }
  int c = 0;
  int limit = strlen(str) - strlen(substr);
  char *p  = str;
  while (*p) {
    if (c > limit) {
      break;
    }
    if (str_starts(p++, substr)) {
      r = c;
    };
    ++c;
  }
  return r;
}
static char *str_left (char *str, int64_t end) {
  return str_sub(str, 0, end);
}
static char *str_ltrim (char *str) {
  unsigned char ch = *str;
  while (ch && ch <= ' ') ch = *++str;
  return str_new(str);
}
static char *str_new (char *s) {
  char *r = ATOMIC(strlen(s) + 1);
  strcpy(r, s);
  return r;
}
static char *str_new_c (char ch) {
  char *r = ATOMIC(2);
  *r = ch;
  r[1] = 0;
  return r;
}
static char *str_replace(char *s, char *old, char *new) {
  if (!*old) return str_new(s);

  Buf *bf = buf_new();
  int len = strlen(old);
  int i = str_index(s, old);
  while (i != -1) {
    buf_add(bf, str_left(s, i));
    buf_add(bf, new);
    s = s + i + len;
    i = str_index(s, old);
  }
  buf_add(bf, s);
  return str_new(buf_str(bf));
}
static char *str_right (char *str, int64_t begin) {
  return str_sub(str, begin, strlen(str));
}
static char *str_next_rune2(char *s) {
  unsigned char b1, b2, b3, b4;
  b1 = *s++;
  if (b1) {
    if (b1 < 0xC2) {
      return str_f("%c", b1);
    } else if (b1 < 0xE0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) {
        return "??";
      }
      return str_f("%c%c", b1, b2);
    } else if (b1 < 0xF0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80 ||
          (b1 == 0xE0 && b2 < 0xA0)
      ) {
        return "??";
      }
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) {
        return "??";
      }
      return str_f("%c%c%c", b1, b2, b3);
    } else if (b1 < 0xF5) {
      b2 = *s++;
      if (((b2 & 0xC0) != 0x80) ||
          (b1 == 0xF0 && b2 < 0x90) ||
          (b1 == 0xF4 && b2 >= 0x90)
      ) {
        return "??";
      }
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) {
        return "??";
      }
      b4 = *s++;
      if ((b4 & 0xC0) != 0x80) {
        return "??";
      }
      return str_f("%c%c%c%c", b1, b2, b3, b4);
    } else {
      return "??";
    }
  }
  return "";
}
static char *str_next_rune (char **rune, char *s) {
  char *r = str_next_rune2(s);
  if (r[0] == '?' && r[1] == '?') {
    *rune = str_new("�");
    return s + 1;
  }
  *rune = r;
  return s + strlen(r);
}
static char *str_rtrim (char *str) {
  char *p = str + (strlen(str) - 1);
  while (p >= str && ((unsigned char)*p) <= ' ') {
    --p;
  }
  return str_left(str, (p - str) + 1);
}
static Arrs *str_split (char *s, char *sep) {
  Arrs *r = tarr_new_s();
  int len = strlen(sep);
  if (!len) return str_to_runes(s);
  int i = str_index(s, sep);
  while (i != -1) {
    char *sub = ATOMIC(i + 1);
    sub[i] = 0;
    memcpy(sub, s, i);
    tarr_pushS(r, sub);
    s = s + i + len;
    i = str_index(s, sep);
  }
  tarr_pushS(r, str_new(s));
  return r;
}
static Arrs *str_split_trim (char *s, char *sep) {
  Arrs *r = str_split(s, sep);
  char **p = tarr_beginS(r);
  char **end = tarr_endS(r);
  while (p < end) {
    *p = str_trim(*p);
    ++p;
  }
  return r;
}
static int str_starts (char *str, char  *substr) {
  while (*substr)
    if (*str++ != *substr++) return 0;
  return 1;
}
static char *str_sub (char *str, int64_t begin, int64_t end) {
  int l = strlen(str);
  if (begin < 0) begin = l + begin;
  if (begin >= l) return "";
  if (begin < 0) begin = 0;

  if (end < 0) end = l + end;
  if (end > l) end = l;
  if (end <= 0) return "";

  int new_l = end - begin;
  if (new_l < 0) return "";

  char *r = ATOMIC(new_l + 1);
  r[new_l] = 0;
  memcpy(r, str + begin, new_l);
  return r;
}
static char *str_to_escape (char *s) {
  Buf *bf = buf_new();
  buf_cadd(bf, '"');
  while (*s) {
    char ch = *s++;
    if (ch == '"' || ch == '\\') {
      buf_cadd(bf, '\\');
    }
    buf_cadd(bf, ch);
  }
  buf_cadd(bf, '"');

  return str_new(buf_str(bf));
}
static char *str_to_lower (char *s) {
  unsigned *ws = str_to_unicode(s);
  unsigned *p = ws;
  while (*p) {
    *p = (unsigned)towlower(*p);
    ++p;
  }
  return str_from_unicode(ws);
}
static Arrs *str_to_runes (char *s) {
  Arrs *r = tarr_new_s();
  char *rune;
  while (*s) {
    s = str_next_rune(&rune, s);
    tarr_pushS(r, rune);
  }
  return r;
}
static unsigned *str_to_unicode (char *s0) {
  unsigned char *s = (unsigned char *)s0;
  unsigned b1, b2, b3, b4;
  int lg = tarr_sizeS(str_to_runes(s0)) + 1;
  unsigned *r = ATOMIC(lg * sizeof(unsigned));
  unsigned *pr = r;
  while (*s) {
    b1 = *s++;
    if (b1 < 0x80) *pr++ = b1;
    else if (b1 < 0xC2) *pr++ = 65533;
    else if (b1 < 0xE0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) *pr++ = 65533;
      *pr++ = (b1 << 6) + b2 - 0x3080;
    } else if (b1 < 0xF0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) *pr++ = 65533;
      if (b1 == 0xE0 && b2 < 0xA0) *pr++ = 65533;
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) *pr++ = 65533;
      *pr++ = (b1 << 12) + (b2 << 6) + b3 - 0xE2080;
    } else if (b1 < 0xF5) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) *pr++ = 65533;
      if (b1 == 0xF0 && b2 < 0x90) *pr++ = 65533;
      if (b1 == 0xF4 && b2 >= 0x90) *pr++ = 65533;
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) *pr++ = 65533;
      b4 = *s++;
      if ((b4 & 0xC0) != 0x80) *pr++ = 65533;
      *pr++ = (b1 << 18) + (b2 << 12) + (b3 << 6) + b4 - 0x3C82080;
    } else {
      *pr++ = 65533;
    }
  }
  *pr = 0;
  return r;
}
static char *str_to_upper (char *s) {
  unsigned *ws = str_to_unicode(s);
  unsigned *p = ws;
  while (*p) {
    *p = (unsigned)towupper(*p);
    ++p;
  }
  return str_from_unicode(ws);
}
static Arri *str_to_utf16 (char *s) {
  unsigned *u = str_to_unicode(s);
  Arri *r = tarr_new_i();
  while (*u) tarr_pushI(r, *u++);
  return r;
}
static char *str_trim (char *s) {
  unsigned char ch = *s;
  while (ch && ch <= ' ') ch = *++s;
  return str_rtrim(s);
}
static char *str_vf(char *format, va_list args) {
  char *tmp;
  int len = vasprintf(&tmp, format, args);
  if (len < 0)
    texc_throw(
      "str.f/str.vf", str_f("Wrong format in:\n  %s'", format)
    );
  char *msg = ATOMIC(len + 1);
  strcpy(msg, tmp);
  free(tmp);
  return msg;
}
