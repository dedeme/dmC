
// regex ---------------------------

// s -> s
Val __regex_matches (char *pos, Val s, Val rg) {
  char *ss = s.s;
  // <RegexOffset>
  Arr *ms = opt_get(regex_matches(rg.s, ss));
  if (!ms)
    ___built_throw(pos, (Val)str_f("Fail compiling regular expression:\n%s", rg));

  Val r = __arr_new();
  int ix = 0;
  EACH(ms, RegexOffset, of) {
    int begin = regexOffset_begin(of);
    int end = regexOffset_end(of);
    __arr_push(r, (Val)str_sub(ss, ix, begin));
    __arr_push(r, (Val)str_sub(ss, begin, end));
    ix = end;
  }_EACH
  __arr_push(r, (Val)str_right(ss, ix));
  return r;
}

// s -> <bytes>
Val __regex_replace (char *pos, Val s, Val rg, Val repl) {
  char *r = opt_get(regex_replace(rg.s, s.s, repl.s));
  if (r) return (Val)r;
  ___built_throw(pos, (Val)str_f("Fail of regular expression:\n%s", rg));
  return (Val)0; // Unreachable
}

// s -> s
Val __regex_replacef (char *pos, Val s, Val rg, Val frpl) {
  char *fn(char *match) {
    return (((Val (*)(Val))frpl.o)((Val)match)).s;
  }
  char *r = opt_get(regex_replacef(rg.s, s.s, fn));
  if (r) return (Val)r;
  ___built_throw(pos, (Val)str_f("Fail of regular expression:\n%s", rg));
  return (Val)0; // Unreachable
}

