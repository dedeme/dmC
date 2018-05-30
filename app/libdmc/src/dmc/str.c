// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <wctype.h>
#include <gc.h>
#include "dmc/str.h"
#include "dmc/Buf.h"
#include "dmc/Achar.h"
#include "dmc/Opt.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

size_t str_len(char *s) {
  XNULL(s);
  return strlen(s);
}

int str_cmp(char *s1, char *s2) {
  XNULL(s1)
  XNULL(s2)
  return strcmp(s1, s2);
}

bool str_eq (char *str1, char *str2) {
  XNULL(str1)
  XNULL(str2)
  return !strcmp(str1, str2);
}

bool str_starts (char *str, char  *substr) {
  XNULL(str)
  XNULL(substr)
  int sublen = strlen(substr);
  return strlen(str) >= sublen && !memcmp(str, substr, sublen);
}

bool str_ends (char *str, char  *substr) {
  XNULL(str)
  XNULL(substr)
  int slen = strlen(str);
  int sublen = strlen(substr);
  return slen >= sublen && !memcmp(str + slen - sublen, substr, sublen);
}

int str_cindex (char *str, char ch) {
  XNULL(str)
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

int str_index (char *str, char *substr) {
  XNULL(str)
  XNULL(substr)

  if (!*substr) {
    return 0;
  }
  int c = 0;
  int limit = strlen(str) - strlen(substr);
  char *p  = str;
  while (*p) {
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

int str_last_cindex (char *str, char ch) {
  XNULL(str)

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

int str_last_index (char *str, char *substr) {
  XNULL(str)
  XNULL(substr)

  int r = -1;
  if (!*substr) {
    return 0;
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

char *str_copy (char *s) {
  XNULL(s)

  char *r = ATOMIC(strlen(s) + 1);
  strcpy(r, s);
  return r;
}

char *str_cat (char *s, ...) {
  XNULL(s)

  va_list args;
  char *tmp;

  Buf *bf = buf_new();
  buf_add(bf, s);

  va_start(args, s);
  tmp = va_arg(args, char *);
  while (tmp) {
    buf_add(bf, tmp);
    tmp = va_arg(args, char *);
  }
  va_end(args);
  return buf_to_str(bf);
}

char *str_sub (char *str, int begin, int end) {
  XNULL(str)

  int l = strlen(str);
  int df = -1;
  if (begin < 0) {
    begin = l + begin;
  }
  if (end < 0) {
    end = l + end;
  }
  df = end - begin;
  if (begin < 0 || end > l) {
    df = -1;
  }

  char *r;
  if (df < 0) {
    char *r = ATOMIC(1);
    *r = 0;
    return r;
  }
  r = ATOMIC(df + 1);
  r[df] = 0;
  memcpy(r, str + begin, df);
  return r;
}

char *str_sub_end (char *str, int begin) {
  XNULL(str)

  return str_sub(str, begin, strlen(str));
}

char *str_ltrim (char *str) {
  XNULL(str)
  while (isspace(*str)) ++str;
  return str_copy(str);
}

char *str_rtrim (char *str) {
  XNULL(str)

  int ix = strlen(str) - 1;
  while (ix >= 0 && isspace(*(str + ix))) {
    --ix;
  }
  return str_sub(str, 0, ix + 1);
}

char *str_trim (char *str) {
  XNULL(str)
  return str_ltrim(str_rtrim(str));
}

Achar *str_csplit (char *s, char sep) {
  XNULL(s)

  Achar *r = achar_new();
  int i;
  while (*s) {
    i = str_cindex(s, sep);
    if (i == -1) break;
    achar_add(r, str_sub(s, 0, i));
    s = s + i + 1;
  }
  if (*s) achar_add(r, s);
  return r;
}

Achar *str_csplit_trim (char *str, char sep) {
  XNULL(str)

  Achar *r = str_csplit(str, sep);
  RANGE0(i, achar_size(r)) {
    achar_set(r, i, str_trim(achar_get(r, i)));
  }_RANGE
  return r;
}

Achar *str_split (char *s, char *sep) {
  XNULL(s)
  XNULL(sep)

  Achar *r = achar_new();
  int len = strlen(sep);
  if (!len) {
    achar_add(r, s);
    return r;
  }
  int i;
  while (*s) {
    i = str_index(s, sep);
    if (i == -1) break;
    achar_add(r, str_sub(s, 0, i));
    s = s + i + len;
  }
  if (*s) achar_add(r, s);
  return r;
}

Achar *str_split_trim (char *str, char *sep) {
  XNULL(str)
  XNULL(sep)

  Achar *r = str_split(str, sep);
  RANGE0(i, achar_size(r)) {
    achar_set(r, i, str_trim(achar_get(r,i)));
  }_RANGE
  return r;
}

char *str_cjoin (Achar *a, char sep) {
  XNULL(a)

  Buf *bf = buf_new();

  if (achar_size(a)) {
    buf_add(bf, achar_get(a, 0));
    RANGE(i, 1, achar_size(a)) {
      buf_cadd(bf, sep);
      buf_add(bf, achar_get(a, i));
    }_RANGE
    return buf_to_str(bf);
  }

  return "";
}

char *str_join (Achar *a, char *sep) {
  XNULL(a)
  XNULL(sep)

  Buf *bf = buf_new();

  if (achar_size(a)) {
    buf_add(bf, achar_get(a, 0));
    RANGE(i, 1, achar_size(a)) {
      buf_add(bf, sep);
      buf_add(bf, achar_get(a, i));
    }_RANGE
    return buf_to_str(bf);
  }

  return "";
}

char *str_creplace(char *s, char old, char new) {
  XNULL(s)

  char *r = str_copy(s);
  char *p = r;
  while (*p) {
    if (*p == old)
      *p = new;
    ++p;
  }
  return r;
}

char *str_replace(char *s, char *old, char *new) {
  XNULL(s)
  XNULL(old)
  XNULL(new)

  if (!*old) return str_copy(s);

  Buf *bf = buf_new();
  int len = strlen(old);
  int ix;

  ix = str_index(s, old);
  while (ix != -1) {
    buf_add(bf, str_sub(s, 0, ix));
    buf_add(bf, new);
    s += ix + len;
    ix = str_index(s, old);
  }
  buf_add(bf, s);
  return buf_to_str(bf);
}

static char *str_vprintf(char *format, va_list args) {
  XNULL(format)

  int length = 0;
  char *rs = (char *)malloc(length);
  va_list args2;
  va_copy(args2, args);
  int nchars = vsnprintf(rs, length, format, args);
  if (nchars >= length) {
    length = nchars + 1;
    free(rs);
    rs = (char *)malloc(length);
    vsnprintf(rs, length, format, args2);
  }
  char *r = ATOMIC(strlen(rs) + 1);
  strcpy(r, rs);
  va_end(args2);
  free(rs);
  return r;
}

char *str_printf(char *format, ...) {
  XNULL(format)

  va_list args;
  va_start(args, format);
  char *r = str_vprintf(format, args);
  va_end(args);
  return r;
}

int str_runes (char *s) {
  XNULL(s)

  unsigned char b1, b2, b3, b4;
  int r = 0;
  while (*s) {
    ++r;
    b1 = *s++;
    if (b1 < 0x80) continue;
    else if (b1 < 0xC2) return -1;
    else if (b1 < 0xE0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return -1;
    } else if (b1 < 0xF0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return -1;
      if (b1 == 0xE0 && b2 < 0xA0) return -1;
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) return -1;
    } else if (b1 < 0xF5) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return -1;
      if (b1 == 0xF0 && b2 < 0x90) return -1;
      if (b1 == 0xF4 && b2 >= 0x90) return -1;
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) return -1;
      b4 = *s++;
      if ((b4 & 0xC0) != 0x80) return -1;
    } else return -1;
  }
  return r;
}

char *str_next_rune (char *s) {
  XNULL(s)

  unsigned char b1, b2, b3, b4;
  b1 = *s++;
  if (b1) {
    if (b1 < 0xC2) {
      return str_printf("%c", b1);
    } else if (b1 < 0xE0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) {
        return str_printf("%c", 0xFF);
      }
      return str_printf("%c%c", b1, b2);
    } else if (b1 < 0xF0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80 ||
          (b1 == 0xE0 && b2 < 0xA0)
      ) {
        return str_printf("%c", 0xFF);
      }
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) {
        return str_printf("%c", 0xFF);
      }
      return str_printf("%c%c%c", b1, b2, b3);
    } else if (b1 < 0xF5) {
      b2 = *s++;
      if (((b2 & 0xC0) != 0x80) ||
          (b1 == 0xF0 && b2 < 0x90) ||
          (b1 == 0xF4 && b2 >= 0x90)
      ) {
        return str_printf("%c", 0xFF);
      }
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) {
        return str_printf("%c", 0xFF);
      }
      b4 = *s++;
      if ((b4 & 0xC0) != 0x80) {
        return str_printf("%c", 0xFF);
      }
      return str_printf("%c%c%c%c", b1, b2, b3, b4);
    } else {
      return str_printf("%c", 0xFF);
    }
  }
  return "";
}

Opt/*unsigned*/ *str_to_unicode (char *s0) {
  XNULL(s0)

  unsigned char *s = (unsigned char *)s0;
  unsigned b1, b2, b3, b4;
  int lg = str_runes(s0) + 1;
  unsigned *r = ATOMIC(lg * sizeof(unsigned));
  unsigned *pr = r;
  while (*s) {
    b1 = *s++;
    if (b1 < 0x80) *pr++ = b1;
    else if (b1 < 0xC2) return opt_null();
    else if (b1 < 0xE0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return opt_null();
      *pr++ = (b1 << 6) + b2 - 0x3080;
    } else if (b1 < 0xF0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return opt_null();
      if (b1 == 0xE0 && b2 < 0xA0) return opt_null();
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) return opt_null();
      *pr++ = (b1 << 12) + (b2 << 6) + b3 - 0xE2080;
    } else if (b1 < 0xF5) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return opt_null();
      if (b1 == 0xF0 && b2 < 0x90) return opt_null();
      if (b1 == 0xF4 && b2 >= 0x90) return opt_null();
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) return opt_null();
      b4 = *s++;
      if ((b4 & 0xC0) != 0x80) return opt_null();
      *pr++ = (b1 << 18) + (b2 << 12) + (b3 << 6) + b4 - 0x3C82080;
    } else {
      return opt_null();
    }
  }
  *pr = 0;
  return opt_new(r);
}

Opt/*char*/ *str_from_unicode (unsigned *u) {
  XNULL(u)

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
      return opt_null();
    }
  }

  return opt_new(buf_to_str(bf));
}

char *str_from_iso (char *s) {
  XNULL(s)

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
  return buf_to_str(bf);
}

char *str_to_upper (char *s) {
  XNULL(s)

  Opt/*unsigned*/ *ows = str_to_unicode(s);
  ONULL(ows)
  unsigned *ws = opt_value(ows);

  unsigned *p = ws;
  while (*p) {
    *p = (unsigned)towupper(*p);
    ++p;
  }

  Opt/*char*/ *r = str_from_unicode(ws);
  ONULL(r)
  return opt_value(r);
}

char *str_to_lower (char *s) {
  XNULL(s)

  Opt/*unsigned*/ *ows = str_to_unicode(s);
  ONULL(ows)
  unsigned *ws = opt_value(ows);

  unsigned *p = ws;
  while (*p) {
    *p = (unsigned)towlower(*p);
    ++p;
  }

  Opt/*char*/ *r = str_from_unicode(ws);
  ONULL(r)
  return opt_value(r);
}

char *str_to_escape (char *s) {
  XNULL(s)

  Buf *bf = buf_new();
  while (*s) {
    char ch = *s++;
    if (ch == '"' || ch == '\\') {
      buf_cadd(bf, '\\');
    }
    buf_cadd(bf, ch);
  }
  return str_printf("\"%s\"", buf_str(bf));
}

char *str_from_escape (char *s) {
  XNULL(s)

  int len = strlen(s);
  if (len < 2) {
    return s;
  }

  char *p = s + 1;
  Buf *bf = buf_new();
  while (*p) {
    char ch = *p++;
    if (ch != '\\') {
      buf_cadd(bf, ch);
    } else {
      buf_cadd(bf, *p++);
    }
  }

  buf_str(bf)[buf_length(bf) - 1] = 0;
  return buf_to_str(bf);
}
