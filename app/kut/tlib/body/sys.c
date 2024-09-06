static void sys_assert (int value, char *pos) {
  if (!value) texc_throw_(str_f("Assert failed\n  %s", pos));
}
static Arrs *sys_cmd (char *c, Arrs *params) {
  Arrs *pars2 = tarr_new_s();
  char **p = tarr_beginS(params);
  char **end = tarr_endS(params);
  while (p < end) tarr_pushS(pars2, str_to_escape(*p++));
  tarr_unshiftS(pars2, c);
  char *command = tarr_join(pars2, " ");
  Arrs *r;;
  char *ferr = file_tmp("/tmp", "dmC");
  char *cmd = str_f("%s 2>%s", command, ferr);
  FILE *fp = popen(cmd, "r");
  if (!fp) {
    file_del(ferr);
    return tarr_new_froms((char*[]){"", str_f("NOEXEC: '%s'", command)}, 2);
  }
  Buf *bf = buf_new();
  char *line = NULL;
  size_t len = 0;
  while (getline(&line, &len, fp) != -1) {
    buf_add(bf, line);
    free(line);
    line = NULL;
  }
  free(line);
  pclose(fp);
  char *err = file_exists(ferr) ? file_read(ferr) : "";
  file_del(ferr);
  return tarr_new_froms((char*[]){str_new(buf_str(bf)), err}, 2);
}
static Arrs *sys_environ(void) {
  Arrs *r = dic_new();
  char **penv = environ;
  while (*penv) {
    Arrs *parts = str_split_trim(*penv++, "=");
    dic_put(r, tarr_getS(parts, 0), tarr_getS(parts, 1));
  }
  return r;
}
static char *sys_get_locale (void) { return setlocale (LC_ALL, 0); }
static char *sys_home (void) { dic_get_or_exc(sys_environ(), "HOME"); }
static void sys_printb (int v) { printf("%s", (built_to_strb(v))); }
static void sys_printi (int64_t v) { printf("%s", (built_to_stri(v))); }
static void sys_printf (double v) { printf("%s", (built_to_strf(v))); }
static void sys_prints (char *v) { printf("%s", (built_to_strs(v))); }
static void sys_printI (Arri *v) { printf("%s", (built_to_strI(v))); }
static void sys_printF (Arrf *v) { printf("%s", (built_to_strF(v))); }
static void sys_printS (Arrs *v) { printf("%s", (built_to_strS(v))); }
static void sys_printD (Arrs *v) { printf("%s", (built_to_strD(v))); }
static void sys_print_errorb (int v) { fprintf(stderr, "%s", (built_to_strb(v))); }
static void sys_print_errori (int64_t v) { fprintf(stderr, "%s", (built_to_stri(v))); }
static void sys_print_errorf (double v) { fprintf(stderr, "%s", (built_to_strf(v))); }
static void sys_print_errors (char *v) { fprintf(stderr, "%s", (built_to_strs(v))); }
static void sys_print_errorI (Arri *v) { fprintf(stderr, "%s", (built_to_strI(v))); }
static void sys_print_errorF (Arrf *v) { fprintf(stderr, "%s", (built_to_strF(v))); }
static void sys_print_errorS (Arrs *v) { fprintf(stderr, "%s", (built_to_strS(v))); }
static void sys_print_errorD (Arrs *v) { fprintf(stderr, "%s", (built_to_strD(v))); }
static void sys_printlnb (int v) { puts(built_to_strb(v)); }
static void sys_printlni (int64_t v) { puts(built_to_stri(v)); }
static void sys_printlnf (double v) { puts(built_to_strf(v)); }
static void sys_printlns (char * v) { puts(built_to_strs(v)); }
static void sys_printlnI (Arri* v) { puts(built_to_strI(v)); }
static void sys_printlnF (Arrf* v) { puts(built_to_strF(v)); }
static void sys_printlnS (Arrs* v) { puts(built_to_strS(v)); }
static void sys_printlnD (Arrs* v) { puts(built_to_strD(v)); }
static char *sys_read_line (void) {
  char *s = malloc(150);
  size_t n = 150;
  if (getline(&s, &n, stdin) == -1) {
    free(s); // correct
    texc_throw("sys.readLine", "Fail reading on console");
  }
  char *r = str_left(s, -1);
  free(s);  // correct
  return r;
}
static void sys_set_locale (char * lang) { setlocale (LC_ALL, lang); }
#define sys_TEST(fneq, fstr) \
  if (fneq(v1, v2)) \
    texc_throw_(str_f( \
      "Test failed\n" \
      "  Expected: %s\n" \
      "    Actual: %s\n" \
      "  %s", \
      fstr(v2), fstr(v1), pos \
    ));
static void sys_sleep (int64_t millis) {
  int64_t end = time_now() + millis;
  struct timespec t;
  struct timespec rem;
  for (;;) {
    t.tv_sec = millis / 1000;
    t.tv_nsec = (millis % 1000) * 1000000;
    if (nanosleep(&t, &rem)) {
      millis = end - time_now();
      if (millis <= 0) break;
    } else {
      break;
    }
  }
}
static void sys_testb (int v1, int v2, char *pos) {
  sys_TEST(built_neqb, built_to_strb)
}
static void sys_testi (int64_t v1, int64_t v2, char *pos) {
  sys_TEST(built_neqi, built_to_stri)
}
static void sys_testf (double v1, double v2, char *pos) {
  sys_TEST(built_neqf, built_to_strf)
}
static void sys_tests (char *v1, char *v2, char *pos) {
  sys_TEST(built_neqs, built_to_strs)
}
static void sys_testo (Obj *v1, Obj *v2, char *pos) {
  sys_TEST(built_neqo, built_to_stro)
}
static void sys_testI (Arri *v1, Arri *v2, char *pos) {
  sys_TEST(built_neqI, built_to_strI)
}
static void sys_testF (Arrf *v1, Arrf *v2, char *pos) {
  sys_TEST(built_neqF, built_to_strF)
}
static void sys_testS (Arrs *v1, Arrs *v2, char *pos) {
  sys_TEST(built_neqS, built_to_strS)
}
static void sys_testD (Arrs *v1, Arrs *v2, char *pos) {
  sys_TEST(built_neqD, built_to_strD)
}
#undef sys_TEST
static void sys_throw (char *fn, char *msg, char *pos) {
  texc_throw_(str_f("%s\n  %s", msg, pos));
}
static void sys_used(void){}
static char *sys_user (void) { dic_get_or_exc(sys_environ(), "USER"); }
