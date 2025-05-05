
// math --------------------------------

// f->f
Val __math_abs (Val n) {
  return (Val)math_abs(n.f);
}

// f->f
Val __math_acos (Val n) {
  return (Val)acos(n.f);
}

// f->f
Val __math_acosh (Val n) {
  return (Val)acosh(n.f);
}

// f->f
Val __math_asin (Val n) {
  return (Val)asin(n.f);
}

// f->f
Val __math_asinh (Val n) {
  return (Val)asinh(n.f);
}

// f->f
Val __math_atan (Val n) {
  return (Val)atan(n.f);
}

// f->f
Val __math_atanh (Val n) {
  return (Val)atanh(n.f);
}

// f->f
Val __math_ceil (Val n) {
  return (Val)ceil(n.f);
}

// f->f
Val __math_cos (Val n) {
  return (Val)cos(n.f);
}

// f->f
Val __math_cosh (Val n) {
  return (Val)cosh(n.f);
}

// ()->f
Val __math_e (void) {
  return (Val)M_E;
}

// fff->b
Val __math_eq (Val n1, Val n2, Val gap) {
  return (Val)math_eq_gap(n1.f, n2.f, gap.f);
}

// f->f
Val __math_exp (Val n) {
  return (Val)exp(n.f);
}
// f->f
Val __math_exp2 (Val n) {
  return (Val)exp2(n.f);
}

// f->f
Val __math_floor (Val n) {
  return (Val)floor(n.f);
}

// s->[f]|[]
Val __math_fromEn (Val s) {
  return __math_stof((Val)str_replace(s.s, ",", ""));
}

// s->[i]|[]
Val __math_fromHex (Val s) {
  char *n = str_trim(s.s);
  if (!*n) return __arr_new();

  char *tmp;
  int64_t r = strtol(n, &tmp, 16);
  if (*tmp) return __arr_new();

  return __rs_some((Val)r);
}

// s->[f]|[]
Val __math_fromIso (Val s) {
  return __math_stof((Val)str_replace(str_replace(s.s, ".", ""), ",", "."));
}
// f->i
Val __math_ftoi (Val n) {
  return (Val)(int64_t)(n.f);
}

// f->s
Val __math_ftos (Val n) {
  return (Val)math_ftos(n.f, 9);
}

// fi->s
Val __math_ftos2 (Val n, Val dec) {
  return (Val)math_ftos(n.f, dec.i);
}

// s->b
Val __math_isDigits (Val n) {
  return (Val)math_digits(n.s);
}

// i->f
Val __math_itof (Val n) {
  return (Val)(double)(n.i);
}

// i->s
Val __math_itoh (Val n) {
  return (Val)str_f("%x", n.i);
}

// i->s
Val __math_itos (Val n) {
  return (Val)math_itos(n.i);
}

// f->f
Val __math_log (Val n) {
  return (Val)log(n.f);
}

// f->f
Val __math_log10 (Val n) {
  return (Val)log10(n.f);
}

// f->f
Val __math_log2 (Val n) {
  return (Val)log2(n.f);
}

// ff->f
Val __math_max (Val n1, Val n2) {
  return n1.f >= n2.f ? n1 : n2;
}

// ()->f
Val __math_maxFloat (void) {
  return (Val)DBL_MAX;
}

// ()->i
Val __math_maxInt (void) {
  return (Val)(int64_t)
    (sizeof(int64_t) == sizeof(long long int) ? LLONG_MAX : LONG_MAX);
}

// ff->f
Val __math_min (Val n1, Val n2) {
  return n1.f <= n2.f ? n1 : n2;
}

// ()->i
Val __math_minInt (void) {
  return (Val)(int64_t)
    (sizeof(int64_t) == sizeof(long long int) ? LLONG_MIN : LONG_MIN);
}

// ()->f
Val __math_pi (void) {
  return (Val)M_PI;
}

// ff->f
Val __math_pow (Val base, Val exp) {
  return (Val)pow(base.f, exp.f);
}

// i->f
Val __math_pow10 (Val exp) {
  int64_t n = exp.i;
  double r = 1;
  double mul = n > 0 ? 10 : 0.1;
  n = n >= 0 ? n : -n;
  REPEAT(n) {
    r *= mul;
  }_REPEAT
  return (Val)r;
}

// ()->f
Val __math_rnd (void) {
  return (Val)((double)rand() / (double)RAND_MAX);
}

// i->i
Val __math_rndi (Val top) {
  return (Val)(int64_t)(((double)rand() / (double)RAND_MAX) * top.i);
}

// fi->f
Val __math_round (Val n, Val dec) {
  return (Val)math_round(n.f, dec.i);
}

// f->f
Val __math_sin (Val n) {
  return (Val)sin(n.f);
}
// f->f
Val __math_sinh (Val n) {
  return (Val)sinh(n.f);
}
// f->f
Val __math_sqrt (Val n) {
  return (Val)sqrt(n.f);
}

// s->[f]|[]
Val __math_stof (Val s) {
  char *n = str_trim(s.s);
  if (!*n) return __arr_new();

  struct lconv *lc = localeconv();
  int ix = str_cindex(n, '.');
  if (ix != -1) n[ix] = *lc->decimal_point;

  char *tmp;
  double r = strtod(n, &tmp);
  if (*tmp) return __arr_new();

  return __rs_some((Val)r);
}

// s->[i]|[]
Val __math_stoi (Val s) {
  char *n = str_trim(s.s);
  if (!*n) return __arr_new();

  char *tmp;
  int64_t r = strtol(n, &tmp, 10);
  if (*tmp) return __arr_new();

  return __rs_some((Val)r);
}

// f->f
Val __math_tan (Val n) {
  return (Val)tan(n.f);
}

// f->f
Val __math_tanh (Val n) {
  return (Val)tanh(n.f);
}

// f->f
Val __math_trunc (Val n) {
  return (Val)trunc(n.f);
}
