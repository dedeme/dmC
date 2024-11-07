// Tmp

void __attribute__ ((constructor)) initLibrary(void) {
}
void __attribute__ ((destructor)) cleanUpLibrary(void) {
}

static void (*built_exc_fns) (char, void *) = NULL;
void built_set_exc(void (*fn)(char, void *)) { // C-Kut connector!!!
  built_exc_fns = fn;
}

static int built_eqb (int v1, int v2) {
  return v1 == v2;
}
static int built_eqi (int64_t v1, int64_t v2) {
  return v1 == v2;
}
static int built_eqf (double v1, double v2) {
  return math_eq(v1, v2);
}
static int built_eqs (char *v1, char *v2) {
  return !strcmp(v1, v2);
}
static int built_eqo (Obj *v1, Obj *v2) {
  return v1->value == v2->value;
}
#define built_EQ(T, BEGIN, END) \
  T *p = BEGIN(v1); \
  T *end = END(v1); \
  T *p2 = BEGIN(v2); \
  if (end - p != END(v2) - p2) return 0;
static int built_eqI (Arri *v1, Arri *v2) {
  built_EQ(int64_t, tarr_beginI, tarr_endI);
  while (p < end) if (*p++ != *p2++) return 0;
  return 1;
}
static int built_eqF (Arrf *v1, Arrf *v2) {
  built_EQ(double, tarr_beginF, tarr_endF);
  while (p < end) if (!math_eq(*p++, *p2++)) return 0;
  return 1;
}
static int built_eqS (Arrs *v1, Arrs *v2) {
  built_EQ(char*, tarr_beginS, tarr_endS);
  while (p < end) if (strcmp(*p++, *p2++)) return 0;
  return 1;
}
static int built_eqD (Arrs *v1, Arrs *v2) {
  built_EQ(char*, tarr_beginS, tarr_endS);
  while (p < end) {
    char *k = *p++;
    char *val = *p++;
    char *val2 = dic_get_or_null(v2, k);
    if (!val2 || strcmp(val, val2)) return 0;
  }
  return 1;
}
#undef built_EQ
static int built_neqb (int v1, int v2) {
  return v1 != v2;
}
static int built_neqi (int64_t v1, int64_t v2) {
  return v1 != v2;
}
static int built_neqf (double v1, double v2) {
  return !math_eq(v1, v2);
}
static int built_neqs (char *v1, char *v2) {
  return strcmp(v1, v2);
}
static int built_neqo (Obj *v1, Obj *v2) {
  return v1->value != v2->value;
}
#define built_NEQ(T, BEGIN, END) \
  T *p = BEGIN(v1); \
  T *end = END(v1); \
  T *p2 = BEGIN(v2); \
  if (end - p != END(v2) - p2) return 1;
static int built_neqI (Arri *v1, Arri *v2) {
  built_NEQ(int64_t, tarr_beginI, tarr_endI);
  while (p < end) if (*p++ != *p2++) return 1;
  return 0;
}
static int built_neqF (Arrf *v1, Arrf *v2) {
  built_NEQ(double, tarr_beginF, tarr_endF);
  while (p < end) if (!math_eq(*p++, *p2++)) return 1;
  return 0;
}
static int built_neqS (Arrs *v1, Arrs *v2) {
  built_NEQ(char*, tarr_beginS, tarr_endS);
  while (p < end) if (strcmp(*p++, *p2++)) return 1;
  return 0;
}
static int built_neqD (Arrs *v1, Arrs *v2) {
  built_NEQ(char*, tarr_beginS, tarr_endS);
  while (p < end) {
    char *k = *p++;
    char *val = *p++;
    char *val2 = dic_get_or_null(v2, k);
    if (!val2 || strcmp(val, val2)) return 1;
  }
  return 0;
}
#undef built_NEQ
static char *built_to_strb (int v) {
  return v ? "true" : "false";
}
static char *built_to_stri (int64_t v) {
  return math_itos(v);
}
static char *built_to_strf (double s) {
  return math_ftos(s);
}
static char *built_to_strs (char *s) {
  return str_new(s);
}
static char *built_to_stro (Obj *o) {
  return str_f("Obj<%s>%ld", o->type, (int64_t)o->value);
}
static char *built_to_strI (Arri *a) {
  Arrs *r = tarr_new_s();
  int64_t *p = tarr_beginI(a);
  int64_t *end = tarr_endI(a);
  while (p < end) tarr_pushS(r, math_itos(*p++));
  char *rs = js_wa(r);
  return rs;
}
static char *built_to_strF (Arrf *a) {
  Arrs *r = tarr_new_s();
  double *p = tarr_beginF(a);
  double *end = tarr_endF(a);
  while (p < end) tarr_pushS(r, math_ftos(*p++));
  char *rs = js_wa(r);
  return rs;
}
static char *built_to_strS (Arrs *a) {
  Arrs *r = tarr_new_s();
  char **p = tarr_beginS(a);
  char **end = tarr_endS(a);
  while (p < end) tarr_pushS(r, js_ws(*p++));
  char *rs = js_wa(r);
  return rs;
}
static char *built_to_strD (Arrs *m) {
  Arrs *r = tarr_new_s();
  char **p = tarr_beginS(m);
  char **end = tarr_endS(m);
  while (p < end) {
    tarr_pushS(r, *p++);
    tarr_pushS(r, js_ws(*p++));
  }
  char *rs = js_wo(r);
  return rs;
}
static void built_trace(int is_full, char *position, char *exp) {
  printf("%s ", position);
  puts(exp);
}
