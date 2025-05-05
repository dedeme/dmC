
// str ---------------------------------

// s,s -> i
Val __str_cmp (Val s1, Val s2) {
  return (Val)(int64_t)strcoll(s1.s, s2.s);
}

// s,s -> b
Val __str_ends (Val str, Val substr) {
  return (Val)str_ends(str.s, substr.s);
}

// s,[s.] -> s
Val __str_fmt (char *pos, Val format, Val args) {
  char *fmt = format.s;
  Varr *as = args.a;

  // Arr<char>
  Arr *pts = arr_new();
  Buf *bf = buf_new();
  char *p = fmt;
  while (*p) {
    char ch = *p++;
    if (ch == '%') {
      ch = *p;
      if (ch == 'v') {
        ++p;
        arr_push(pts, buf_str(bf));
        bf = buf_new();
        continue;
      }
      if (ch == '%') ++p;
    }
    buf_cadd(bf, ch);
  }
  arr_push(pts, buf_str(bf));

  if (arr_size(pts) != __arr_size(args).i + 1)
    ___built_throw(pos, (Val)str_f(
      "Wrong arguments number (expected %d, fund %d)",
      arr_size(pts) - 1, __arr_size(args).i
    ));

  // Arr<char>
  Arr *rs = arr_new();
  void **ps = arr_begin(pts);
  arr_push(rs, *ps++);
  Val *pas = as->begin;
  Val *pend = as->end;
  while (pas < pend) {
    arr_push(rs, pas++->s);
    arr_push(rs, *ps++);
  }
  return (Val)arr_join(rs, "");
}

// s -> s
Val __str_fromIso (Val s) {
  return (Val)str_from_iso(s.s);
}

// [.s] -> s
Val __str_fromRunes (Val a) {
  return __arr_join(a, (Val)"");
}

// [.i] -> s
Val __str_fromUtf16 (Val codepoints) {
  int64_t sz = __arr_size(codepoints).i;
  unsigned *u = ATOMIC(sizeof(unsigned) * (sz + 1));
  Val *p = codepoints.a->begin;
  Val *end = codepoints.a->end;
  unsigned *pu = u;
  while (p < end) *pu++ = (unsigned)(*p++).i;
  *pu = 0;
  return (Val)str_from_unicode(u);

};

// s,s,i -> s
Val __str_get(char *pos, Val s, Val ix) {
  if (ix.i < 0 || ix.i >= strlen(s.s))
    ___built_throw(pos, (Val)str_f("Index %d out of [0,%d]", ix.i, strlen(s.s) - 1));
  return (Val)str_new_c(s.s[ix.i]);
}

// s,s -> i
Val __str_index(Val str, Val substr) {
  return (Val)(int64_t)str_index(str.s, substr.s);
}

// s,s,i -> i
Val __str_indexFrom (Val str, Val substr, Val start) {
  return (Val)(int64_t)str_index_from(str.s, substr.s, start.i);
}

// s,s -> i
Val __str_lastIndex (Val str, Val substr) {
  return (Val)(int64_t)str_last_index(str.s, substr.s);
}

// s,i -> [.s]
Val __str_left (Val str, Val end) {
  Val r = __str_toRunes(str);
  int64_t sz = __arr_size(r).i;
  end = end.i < 0 ? (Val)(sz + end.i) : end;
  return __arr_join(__arr_take(r, end), (Val)"");
}

// s,i -> s
Val __str_left_slice(Val str, Val begin) {
  return (Val)str_left(str.s, begin.i);
}

// s -> i
Val __str_len (Val str) {
  return (Val)(int64_t)strlen(str.s);
}

// s -> s
Val __str_ltrim (Val str) {
  return (Val)str_ltrim(str.s);
}


// s,s,s -> s
Val __str_replace (Val s, Val old, Val new) {
  return (Val)str_replace(s.s, old.s, new.s);
}

// s,i -> [.s]
Val __str_right (Val str, Val begin) {
  Val r = __str_toRunes(str);
  int64_t sz = __arr_size(r).i;
  begin = begin.i < 0 ? (Val)(sz + begin.i) : begin;
  return __arr_join(__arr_drop(r, begin), (Val)"");
}

// s,i -> s
Val __str_right_slice(Val str, Val begin) {
  return (Val)str_right(str.s, begin.i);
}

// s -> s
Val __str_rtrim (Val str) {
  return (Val)str_rtrim(str.s);
}

// s,s -> [s.]
Val __str_split (Val str, Val sep) {
  char *s = str.s;
  char *ssep = sep.s;
  Val r = __arr_new();
  int len = strlen(ssep);

  if (!len) {
    char *rune;
    s = str_next_rune(&rune, s);
    while (*rune) {
      __arr_push(r, (Val)rune);
      s = str_next_rune(&rune, s);
    }
    return r;
  }

  if (len == 1) {
    char csep = *ssep;
    int i = str_cindex(s, csep);
    while (i != -1) {
      char *sub = ATOMIC(i + 1);
      sub[i] = 0;
      memcpy(sub, s, i);
      __arr_push(r, (Val)sub);
      s = s + i + len;
      i = str_cindex(s, csep);
    }
    __arr_push(r, (Val)str_new(s));
    return r;
  }

  int i = str_index(s, ssep);
  while (i != -1) {
    char *sub = ATOMIC(i + 1);
    sub[i] = 0;
    memcpy(sub, s, i);
    __arr_push(r, (Val)sub);
    s = s + i + len;
    i = str_index(s, ssep);
  }
  __arr_push(r, (Val)str_new(s));
  return r;
}

// s,s -> [s.]
Val __str_splitTrim (Val str, Val sep) {
  Val r = __str_split(str, sep);
  Val *p = r.a->begin;
  Val *end = r.a->end;
  while (p < end) {
    *p = __str_trim(*p);
    ++p;
  }
  return r;
}

// s,s -> b
Val __str_starts(Val str, Val substr) {
  return (Val)str_starts(str.s, substr.s);
}

// s,i,i -> [.s]
Val __str_sub (Val str, Val begin, Val end) {
  Val r = __str_toRunes(str);
  int64_t sz = __arr_size(r).i;
  begin = begin.i < 0 ? (Val)(sz + begin.i) : begin;
  end = end.i < 0 ? (Val)(sz + end.i) : end;
  return __arr_join(__arr_drop(__arr_take(r, end), begin), (Val)"");
}

// s,i,i -> s
Val __str_sub_slice(Val str, Val begin, Val end) {
  return (Val)str_sub(str.s, begin.i, end.i);
}

// s -> s
Val __str_toLower(Val s) {
  return (Val)str_to_lower(s.s);
}

// s -> s
Val __str_toUpper(Val s) {
  return (Val)str_to_upper(s.s);
}

// s -> [.s]
Val __str_toRunes (Val str) {
  char *s = str.s;
  Val r = __arr_new();
  char *rune;
  s = str_next_rune(&rune, s);
  while (*rune) {
    __arr_push(r, (Val)rune);
    s = str_next_rune(&rune, s);
  }
  return r;
}

// s -> [.i]
Val __str_toUtf16 (Val str) {
  unsigned *u = str_to_unicode(str.s);
  Val a = __arr_new();
  unsigned *pu = u;
  while (*pu) __arr_push(a, (Val)(int64_t)*pu++);
  return a;
}

// s -> s
Val __str_trim (Val str) {
  return (Val)str_trim(str.s);
}

