typedef struct regex_RegexOffset {
  int begin;
  int end;
} RegexOffset;
static RegexOffset *regexOffset_new(int begin, int end) {
  RegexOffset *this = MALLOC(RegexOffset);
  this->begin = begin;
  this->end = end;
  return this;
}
static Arrs *regex_matches (char *s, char *rg) {
  Arrs *r = tarr_new_s();

  regex_t rex;
  if (regcomp(&rex, rg, REG_EXTENDED))
    texc_throw("regex.matches", "Regular expression can not be compiled");
  regmatch_t match;

  int ix = 0;
  for (;;) {
    char *sub = str_right(s, ix);
    char *offset = NULL;
    int rs = regexec(&rex, sub, 1, &match, 0);
    if (rs == REG_ESPACE)
      texc_throw("regex.matches", "Regular expression out of memory");

    if (!rs) {
      tarr_pushS(r, str_sub(s, ix, ix + match.rm_so));
      int end = ix + match.rm_eo;
      offset = str_sub(s, ix + match.rm_so, end);
      ix = end;
    }

    if (offset) tarr_pushS(r, offset);
    else { tarr_pushS(r, str_right(s, ix)); break; }
  }

  regfree(&rex);
  return r;
}
static char *regex_replace (char *source, char *rg, char *repl) {
  Arrs *ms = regex_matches(source, rg);
  char **p = tarr_beginS(ms);
  char **end = tarr_endS(ms);
  Arrs *r = tarr_new_s();
  for (;;) {
    if (p >= end) break;
    tarr_pushS(r, *p++);
    if (p >= end) break;
    tarr_pushS(r, repl);
    ++p;
  }
  return tarr_join(r, "");
}
