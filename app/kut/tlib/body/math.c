static double math_abs (double n) {
  return n >= 0 ? n : -n;
}
static int math_digits (char *s) {
  if (!*s) return 0;
  char *ps = s;
  char ch;
  while ((ch = *ps++)) if (ch < '0' || ch > '9') return 0;
  return 1;
}
static int math_eq (double n1, double n2) {
  return math_eq_gap(n1, n2, 0.0000001);
}
static int math_eq_gap (double n1, double n2, double gap) {
  gap = gap > 1 ? 1 : gap < 0.0000000001 ? 0.0000000001 : gap;
  return n1 < n2 + gap && n1 > n2 - gap;
}
static Arrf *math_from_en (char *s) {
  return math_stof(str_replace(s, ",", ""));
}
static Arri *math_from_hex (char *s) {
  return math_stoi_base(s, 16);
}
static Arrf *math_from_iso (char *s) {
  return math_stof(str_replace(str_replace(s, ".", ""), ",", "."));
}
static char *math_ftos (double n) {
  return math_ftos2(n, 9);
}
static char *math_ftos2 (double n, int scale) {
  scale = scale < 0 ? 0 : scale > 9 ? 9 : scale;
  char *tpl = str_f("%%.%df", scale);
  char *ns = str_f(tpl, n + (n >= 0 ? 0.000000000001 : -0.000000000001));
  if (scale > 0) {
    char *p = ns;
    while (*p) {
      if (*p == ',') *p++ = '.';
      else ++p;
    }
    --p;
    while (*p == '0') --p;
    if (*p != '.') ++p;
    ns = str_left(ns, p - ns);
  }
  if (!strcmp(ns, "-0")) ns = "0";
  return ns;
}
static char *math_itos (int64_t n) {
  return str_f("%ld", n);
}
static double math_max (double n1, double n2) {
  return n1 > n2 ? n1 : n2;
}
static double math_min (double n1, double n2) {
  return n1 < n2 ? n1 : n2;
}
static double math_pow10 (int64_t ex) {
  double r = 1;
  double mul = ex > 0 ? 10 : 0.1;
  ex = (ex >= 0 ? ex : -ex) + 1;
  while(--ex) {
    r *= mul;
  }
  return r;
}
static double math_rnd (void) {
  return (double)rand() / (double)RAND_MAX;
}
static int64_t math_rndi (int64_t top) {
  return (int)(math_rnd() * top);
}
static double math_round (double n, int64_t scale) {
  n = n >= 0 ? n + 0.00000000001 : n - 0.00000000001;
  switch (scale) {
    case 0: return round(n);
    case 1: return round (n * 10.0) / 10.0;
    case 2: return round (n * 100.0) / 100.0;
    case 3: return round (n * 1000.0) / 1000.0;
    case 4: return round (n * 10000.0) / 10000.0;
    case 5: return round (n * 100000.0) / 100000.0;
    case 6: return round (n * 1000000.0) / 1000000.0;
    case 7: return round (n * 10000000.0) / 10000000.0;
    case 8: return round (n * 100000000.0) / 100000000.0;
    case 9: return round (n * 1000000000.0) / 1000000000.0;
    default: if (scale < 0) return round(n);
             else return round (n * 1000000000.0) / 1000000000.0;
  }
}
static Arrf *math_stof (char *s) {
  s = str_trim(s);
  if (!*s) return tarr_new_f();

  struct lconv *lc = localeconv();
  int ix = str_cindex(s, '.');
  if (ix != -1) s[ix] = *lc->decimal_point;

  char *tmp;
  double r = strtod(s, &tmp);
  if (*tmp) return tarr_new_f();
  return tarr_newf(r, 1);
}
static Arri *math_stoi (char *s) {
  math_stoi_base(s, 0);
}
static Arri *math_stoi_base (char *s, int base) {
  s = str_trim(s);
  if (!*s) return tarr_new_i();

  char *tmp;
  int64_t r = strtol(s, &tmp, base);
  if (*tmp) return tarr_new_i();

  return tarr_newi(r, 1);
}
