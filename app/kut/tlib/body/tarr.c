struct tarr_Arri {
  int64_t *begin;
  int64_t *end;
  int64_t *bf;
};
struct tarr_Arrf {
  double *begin;
  double *end;
  double *bf;
};
struct tarr_Arrs {
  char **begin;
  char **end;
  char **bf;
};
struct tarr_Arr {
  void **begin;
  void **end;
  void **bf;
};

static int64_t *tarr_beginI (Arri *this) { return this->begin; }
static double *tarr_beginF (Arrf *this) { return this->begin; }
static char **tarr_beginS (Arrs *this) { return this->begin; }
#define tarr_CAT(Vtype) \
  int other_len = other->end - other->begin; \
  if (other_len) { \
    int this_len = this->end - this->begin; \
    int this_size = this->bf - this->begin; \
    if (this_len + other_len >= this_size){ \
      int new_size = this_size + other_len; \
      this->begin = GC_REALLOC(this->begin, new_size * sizeof(Vtype)); \
      this->end = this->begin + this_len; \
      this->bf = this->begin + new_size; \
    } \
    Vtype *s = other->begin; \
    Vtype *t = this->end; \
    while (s < other->end) *t++ = *s++; \
    this->end = t; \
  }
static void tarr_catI (Arri *this, Arri *other) { tarr_CAT(int64_t) }
static void tarr_catF (Arrf *this, Arrf *other) { tarr_CAT(double) }
static void tarr_catS (Arrs *this, Arrs *other) { tarr_CAT(char*) }
#undef tarr_CAT
static void tarr_clearI (Arri *this) {
  this->end = this->begin;
}
static void tarr_clearF (Arrf *this) {
  this->end = this->begin;
}
static void tarr_clearS (Arrs *this) {
  this->end = this->begin;
}
static Arri *tarr_concatI (Arri *this, Arri *other) {
  Arri *r = tarr_copyI(this);
  tarr_catI(r, other);
  return r;
}
static Arrf *tarr_concatF (Arrf *this, Arrf *other) {
  Arrf *r = tarr_copyF(this);
  tarr_catF(r, other);
  return r;
}
static Arrs *tarr_concatS (Arrs *this, Arrs *other) {
  Arrs *r = tarr_copyS(this);
  tarr_catS(r, other);
  return r;
}
static int tarr_containsI (Arri *this, int64_t e) {
  int64_t *p = this->begin; int64_t *end = this->end;
  while (p < end) if (*p++ == e) return 1;
  return 0;
}
static int tarr_containsF (Arrf *this, double e) {
  double *p = this->begin; double *end = this->end;
  while (p < end) if (math_eq(*p++, e)) return 1;
  return 0;
}
static int tarr_containsS (Arrs *this, char *e) {
  char **p = this->begin; char **end = this->end;
  while (p < end) if (!strcmp(*p++, e)) return 1;
  return 0;
}
#define tarr_COPY(Atype, Vtype, ALLOC_fn) \
  int size = this->end - this->begin; \
  int bf_size = this->bf - this->begin; \
  Atype *a = MALLOC(Atype); \
  a->begin = ALLOC_fn(sizeof(Vtype) * bf_size); \
  a->end = a->begin + size; \
  a->bf = a->begin + bf_size; \
  memcpy(a->begin, this->begin, sizeof(Vtype) * size); \
  return a;
static Arri *tarr_copyI (Arri *this) { tarr_COPY(Arri, int64_t, ATOMIC) }
static Arrf *tarr_copyF (Arrf *this) { tarr_COPY(Arrf, double, ATOMIC) }
static Arrs *tarr_copyS (Arrs *this) { tarr_COPY(Arrs, char*, GC_MALLOC) }
#undef tarr_COPY
#define tarr_DROP(Atype, Vtype, ALLOC_fn) \
  int size = this->end - this->begin; \
  if (n < 0) n = 0; \
  else if (n > size) n = size; \
  int new_size = size - n; \
  Atype *r = MALLOC(Atype); \
  int bf_size = this->bf - this->begin; \
  r->begin = ALLOC_fn(sizeof(Vtype) * bf_size); \
  r->end = r->begin + new_size; \
  r->bf = r->begin + bf_size; \
  if (new_size > 0) memcpy(r->begin, this->begin + n, sizeof(Vtype) * new_size); \
  return r;
static Arri *tarr_dropI (Arri *this, int64_t n) { tarr_DROP(Arri, int64_t, ATOMIC) }
static Arrf *tarr_dropF (Arrf *this, int64_t n) { tarr_DROP(Arrf, double, ATOMIC) }
static Arrs *tarr_dropS (Arrs *this, int64_t n) { tarr_DROP(Arrs, char*, GC_MALLOC) }
#undef tarr_DROP
#define tarr_DUPLICATES(Atype, Vtype, copy_fn, new_fn, clear_fn, eq_fn, push_fn) \
  Atype *old = copy_fn(this); \
  Atype *dup = new_fn(); \
  clear_fn(this); \
  Vtype *pold = old->begin; \
  Vtype *end_old = old->end; \
  while (pold < end_old) { \
    Vtype e = *pold++; \
    Vtype *pdup = dup->begin; \
    Vtype *end_dup = dup->end; \
    int is_dup = 0; \
    while (pdup < end_dup) \
      if (eq_fn(*pdup++, e)) { is_dup = 1; break; } \
    if (is_dup) continue; \
    Vtype *p = this->begin; \
    Vtype *end = this->end; \
    while (p < end) \
      if (eq_fn(*p++, e)) { is_dup = 1; break; } \
    if (is_dup) push_fn(dup, e); \
    else push_fn(this, e); \
  } \
  return dup;
static Arri *tarr_duplicatesI (Arri *this) {
  tarr_DUPLICATES(
    Arri, int64_t, tarr_copyI, tarr_new_i, tarr_clearI, built_eqi, tarr_pushI
  )
}
static Arrf *tarr_duplicatesF (Arrf *this) {
  tarr_DUPLICATES(
    Arrf, double, tarr_copyF, tarr_new_f, tarr_clearF, built_eqf, tarr_pushF
  )
}
static Arrs *tarr_duplicatesS (Arrs *this) {
  tarr_DUPLICATES(
    Arrs, char*, tarr_copyS, tarr_new_s, tarr_clearS, built_eqs, tarr_pushS
  )
}
#undef tarr_DUPLICATES
static int64_t *tarr_endI (Arri *this) { return this->end; }
static double *tarr_endF (Arrf *this) { return this->end; }
static char **tarr_endS (Arrs *this) { return this->end; }
static int tarr_empty (Arr *this) { return this->begin == this->end; }
static int tarr_emptyI (Arri *this) { return this->begin == this->end; }
static int tarr_emptyF (Arrf *this) { return this->begin == this->end; }
static int tarr_emptyS (Arrs *this) { return this->begin == this->end; }
static int64_t tarr_getI (Arri *this, int64_t ix) {
  texc_check_range("arr.get", 0, (this->end - this->begin) - 1, ix);
  return *(this->begin + ix);
}
static double tarr_getF (Arrf *this, int64_t ix) {
  texc_check_range("arr.get", 0, (this->end - this->begin) - 1, ix);
  return *(this->begin + ix);
}
static char *tarr_getS (Arrs *this, int64_t ix) {
  texc_check_range("arr.get", 0, (this->end - this->begin) - 1, ix);
  return *(this->begin + ix);
}
static int64_t tarr_indexI (Arri *this, int64_t e) {
  int64_t *p = this->begin; int64_t *end = this->end;
  while (p < end) if (*p++ == e) return (p - this->begin) - 1;
  return -1;
}
static int64_t tarr_indexF (Arrf *this, double e) {
  double *p = this->begin; double *end = this->end;
  while (p < end) if (math_eq(*p++, e)) return (p - this->begin) - 1;
  return -1;
}
static int64_t tarr_indexS (Arrs *this, char *e) {
  char **p = this->begin; char **end = this->end;
  while (p < end) if (!strcmp(*p++, e)) return (p - this->begin) - 1;
  return -1;
}
#define tarr_INSERT(Atype, Vtype, fn_push) \
  texc_check_range("arr.insert", 0, this->end - this->begin, ix); \
  fn_push(this, e); \
  Vtype *p = this->end - 1; \
  Vtype *end = this->begin + ix; \
  while (p > end) { \
    *p = *(p - 1); \
    --p; \
  } \
  *end = e;
static void tarr_insertI (Arri *this, int64_t ix, int64_t e) {
  tarr_INSERT(Arri, int64_t, tarr_pushI);
}
static void tarr_insertF (Arrf *this, int64_t ix, double e) {
  tarr_INSERT(Arrf, double, tarr_pushF);
}
static void tarr_insertS (Arrs *this, int64_t ix, char *e) {
  tarr_INSERT(Arrs, char*, tarr_pushS);
}
#undef tarr_INSERT
static void tarr_insert_arrayI (Arri *this, int64_t ix, Arri *other) {
  tarr_insert_rangeI(this, ix, other, 0, tarr_sizeI(other));
}
static void tarr_insert_arrayF (Arrf *this, int64_t ix, Arrf *other) {
  tarr_insert_rangeF(this, ix, other, 0, tarr_sizeF(other));
}
static void tarr_insert_arrayS (Arrs *this, int64_t ix, Arrs *other) {
  tarr_insert_rangeS(this, ix, other, 0, tarr_sizeS(other));
}
#define tarr_INSERT_RANGE(Atype, Vtype, fn_drop, fn_size, fn_set_range, fn_set_array) \
  Atype *tmp = fn_drop(this, ix); \
  int64_t size = fn_size(this) + end - begin; \
  int64_t bf = size + 15; \
  this->begin = GC_REALLOC(this->begin, bf * sizeof(Vtype)); \
  this->end = this->begin + size; \
  this->bf = this->begin + bf; \
  fn_set_range(this, ix, other, begin, end); \
  fn_set_array(this, ix + end - begin, tmp);
static void tarr_insert_rangeI (
  Arri *this, int64_t ix, Arri *other, int64_t begin, int64_t end
) {
  tarr_INSERT_RANGE(
    Arri, int64_t, tarr_dropI, tarr_sizeI, tarr_set_rangeI, tarr_set_arrayI
  );
}
static void tarr_insert_rangeF (
  Arrf *this, int64_t ix, Arrf *other, int64_t begin, int64_t end
) {
  tarr_INSERT_RANGE(
    Arrf, double, tarr_dropF, tarr_sizeF, tarr_set_rangeF, tarr_set_arrayF
  );
}
static void tarr_insert_rangeS (
  Arrs *this, int64_t ix, Arrs *other, int64_t begin, int64_t end
) {
  tarr_INSERT_RANGE(
    Arrs, char*, tarr_dropS, tarr_sizeS, tarr_set_rangeS, tarr_set_arrayS
  );
}
#undef tarr_INSERT_RANGE
static int64_t tarr_ix_correctI (Arri *this, int64_t n) {
  return n < 0 ? (this->end - this->begin) + n : n;
}
static int64_t tarr_ix_correctF (Arrf *this, int64_t n) {
  return n < 0 ? (this->end - this->begin) + n : n;
}
static int64_t tarr_ix_correctS (Arrs *this, int64_t n) {
  return n < 0 ? (this->end - this->begin) + n : n;
}
static char *tarr_join (Arrs *this, char *sep) {
  Buf *bf = buf_new();
  int first = 1;
  char **p = (char **)this->begin;
  while (p < (char **)this->end) {
    if (first) {
      first = 0;
    } else {
      buf_add(bf, sep);
    }
    buf_add(bf, *p++);
  }
  char *r = str_new(buf_str(bf));
  return r;
}
#define tarr_NEW(Atype, Vtype, ALLOC_fn) \
  Atype *this = MALLOC(Atype); \
  this->begin = ALLOC_fn(sizeof(Vtype) * 15); \
  this->end = this->begin; \
  this->bf = this->begin + 15; \
  return this;
static Arr *tarr_new_ (void) { tarr_NEW(Arr, void*, GC_MALLOC) }
static Arri *tarr_new_i (void) { tarr_NEW(Arri, int64_t, ATOMIC) }
static Arrf *tarr_new_f (void) { tarr_NEW(Arrf, double, ATOMIC) }
static Arrs *tarr_new_s (void) { tarr_NEW(Arrs, char*, GC_MALLOC) }
#undef tarr_NEW
#define tarr_NEW_FROM_C(Atype, Vtype, ALLOC_fn) \
  int bf_size = n < 15 ? 15 : n; \
  Atype *this = MALLOC(Atype); \
  this->begin = ALLOC_fn(sizeof(Vtype) * bf_size); \
  memcpy(this->begin, es, sizeof(Vtype) * n); \
  this->end = this->begin + n; \
  this->bf = this->begin + bf_size; \
  return this;
static Arri *tarr_new_fromi (int64_t *es, int64_t n) {
  tarr_NEW_FROM_C(Arri, int64_t, ATOMIC)
}
static Arrf *tarr_new_fromf (double *es, int64_t n) {
  tarr_NEW_FROM_C(Arrf, double, ATOMIC)
}
static Arrs *tarr_new_froms (char **es, int64_t n) {
  tarr_NEW_FROM_C(Arrs, char*, GC_MALLOC)
}
#undef tarr_NEW_FROM_C
static Arri *tarr_newi (int64_t e, int64_t n) {
  if (n < 0) n = 0;
  int64_t *es = ATOMIC(sizeof(int64_t) * n);
  int64_t *p = es;
  int64_t *end = es + n;
  while (p < end) *p++ = e;
  return tarr_new_fromi(es, n);
}
static Arrf *tarr_newf (double e, int64_t n) {
  if (n < 0) n = 0;
  double *es = ATOMIC(sizeof(double) * n);
  double *p = es;
  double *end = es + n;
  while (p < end) *p++ = e;
  return tarr_new_fromf(es, n);
}
static Arrs *tarr_news (char *e, int64_t n) {
  if (n < 0) n = 0;
  char **es = ATOMIC(sizeof(char **) * n);
  char **p = es;
  char **end = es + n;
  while (p < end) *p++ = e;
  return tarr_new_froms(es, n);
}
#define tarr_PEEK \
  if (this->begin == this->end) texc_throw("arr.peek", "Array is empty"); \
  return *(this->end - 1);
static void *tarr_peek (Arr *this) { tarr_PEEK }
static int64_t tarr_peekI (Arri *this)  { tarr_PEEK }
static double tarr_peekF (Arrf *this)  { tarr_PEEK }
static char *tarr_peekS (Arrs *this) { tarr_PEEK }
#undef tarr_PEEK
#define tarr_POP \
  if (this->begin == this->end) texc_throw("arr.pop", "Array is empty"); \
  return *(--this->end);
static void *tarr_pop (Arr *this) { tarr_POP }
static int64_t tarr_popI (Arri *this) { tarr_POP }
static double tarr_popF (Arrf *this) { tarr_POP }
static char *tarr_popS (Arrs *this) { tarr_POP }
#undef tarr_POP
#define tarr_PUSH(Vtype) \
  if (this->end == this->bf) { \
    int size = this->bf - this->begin; \
    int new_size = size + size; \
    this->begin = GC_REALLOC(this->begin, new_size * sizeof(Vtype)); \
    this->end = this->begin + size; \
    this->bf = this->begin + new_size; \
  } \
  *this->end++ = e;
static void tarr_push (Arr *this, void *e) { tarr_PUSH(void *); }
static void tarr_pushI (Arri *this, int64_t e) { tarr_PUSH(int64_t); }
static void tarr_pushF (Arrf *this, double e) { tarr_PUSH(double); }
static void tarr_pushS (Arrs *this, char *e) { tarr_PUSH(char *); }
#undef tarr_PUSH
#define tarr_REMOVE(Atype, Vtype, copy_fn, clear_fn, cat_fn, take_fn, drop_fn) \
  texc_check_range("arr.remove", 0, (this->end - this->begin) - 1, ix); \
  Vtype r = *(this->begin + ix); \
  Atype *tmp = copy_fn(this); \
  clear_fn(this); \
  cat_fn(this, take_fn(tmp, ix)); \
  cat_fn(this, drop_fn(tmp, ix + 1)); \
  return r;
static int64_t tarr_removeI (Arri *this, int64_t ix) {
  tarr_REMOVE(
    Arri, int64_t, tarr_copyI, tarr_clearI, tarr_catI, tarr_takeI, tarr_dropI
  )
}
static double tarr_removeF (Arrf *this, int64_t ix) {
  tarr_REMOVE(
    Arrf, double, tarr_copyF, tarr_clearF, tarr_catF, tarr_takeF, tarr_dropF
  )
}
static char *tarr_removeS (Arrs *this, int64_t ix) {
  tarr_REMOVE(
    Arrs, char *, tarr_copyS, tarr_clearS, tarr_catS, tarr_takeS, tarr_dropS
  )
}
#undef tarr_REMOVE
#define tarr_REMOVE_RANGE(Atype, copy_fn, clear_fn, cat_fn, take_fn, drop_fn) \
  texc_check_range("arr.removeRange", 0, (this->end - this->begin), end); \
  texc_check_range("arr.removeRange", 0, end, begin); \
  Atype *r = drop_fn(take_fn(this, end), begin); \
  Atype *tmp = copy_fn(this); \
  clear_fn(this); \
  cat_fn(this, take_fn(tmp, begin)); \
  cat_fn(this, drop_fn(tmp, end)); \
  return r;
static Arri *tarr_remove_rangeI (Arri *this, int64_t begin, int64_t end) {
  tarr_REMOVE_RANGE(
    Arri, tarr_copyI, tarr_clearI, tarr_catI, tarr_takeI, tarr_dropI
  )
}
static Arrf *tarr_remove_rangeF (Arrf *this, int64_t begin, int64_t end) {
  tarr_REMOVE_RANGE(
    Arrf, tarr_copyF, tarr_clearF, tarr_catF, tarr_takeF, tarr_dropF
  )
}
static Arrs *tarr_remove_rangeS (Arrs *this, int64_t begin, int64_t end) {
  tarr_REMOVE_RANGE(
    Arrs, tarr_copyS, tarr_clearS, tarr_catS, tarr_takeS, tarr_dropS
  )
}
#undef tarr_REMOVE_RANGE
static Arri *tarr_reverseI (Arri *this) {
  Arri *r = tarr_copyI(this);
  tarr_reverse_inI(r);
  return r;
}
static Arrf *tarr_reverseF (Arrf *this) {
  Arrf *r = tarr_copyF(this);
  tarr_reverse_inF(r);
  return r;
}
static Arrs *tarr_reverseS (Arrs *this) {
  Arrs *r = tarr_copyS(this);
  tarr_reverse_inS(r);
  return r;
}
#define tarr_REVERSE_IN(Vtype) \
  Vtype *p = this->begin; \
  Vtype *end = this->end - 1; \
  int size2 = (this->end - this->begin) / 2; \
  Vtype tmp; \
  for (int i = 0; i < size2; ++i) { \
    tmp = *p; \
    *p++ = *end; \
    *end-- = tmp; \
  }
static void tarr_reverse_inI (Arri *this) { tarr_REVERSE_IN(int64_t) }
static void tarr_reverse_inF (Arrf *this) { tarr_REVERSE_IN(double) }
static void tarr_reverse_inS (Arrs *this) { tarr_REVERSE_IN(char*) }
#undef tarr_REVERSE_IN
static void tarr_setI (Arri *this, int64_t ix, int64_t e) {
  texc_check_range("arr.set", 0, (this->end - this->begin) - 1, ix);
  *(this->begin + ix) = e;
}
static void tarr_setF (Arrf *this, int64_t ix, double e) {
  texc_check_range("arr.set", 0, (this->end - this->begin) - 1, ix);
  *(this->begin + ix) = e;
}
static void tarr_setS (Arrs *this, int64_t ix, char *e) {
  texc_check_range("arr.set", 0, (this->end - this->begin) - 1, ix);
  *(this->begin + ix) = e;
}
static void tarr_set_arrayI (Arri *this, int64_t ix, Arri *other) {
  tarr_set_rangeI(this, ix, other, 0, tarr_sizeI(other));
}
static void tarr_set_arrayF (Arrf *this, int64_t ix, Arrf *other) {
  tarr_set_rangeF(this, ix, other, 0, tarr_sizeF(other));
}
static void tarr_set_arrayS (Arrs *this, int64_t ix, Arrs *other) {
  tarr_set_rangeS(this, ix, other, 0, tarr_sizeS(other));
}
#define tarr_SET_RANGE(Vtype, fn_size) \
  if (end < begin) \
    texc_expect("arr.setRange", \
      "end < begin", str_f("end == %d", end), str_f("end >= %d", begin) \
    ); \
  int size = end - begin; \
  if (!size) return; \
  texc_check_range("arr.setRange", 0, fn_size(other) - size, begin); \
  texc_check_range("arr.setRange", 0, fn_size(this) - size, ix); \
  Vtype *target = this->begin + ix; \
  Vtype *source = other->begin + begin; \
  memcpy(target, source, sizeof(void *) * size);
static void tarr_set_rangeI (
  Arri *this, int64_t ix, Arri *other, int64_t begin, int64_t end
) { tarr_SET_RANGE(int64_t, tarr_sizeI) }
static void tarr_set_rangeF (
  Arrf *this, int64_t ix, Arrf *other, int64_t begin, int64_t end
) { tarr_SET_RANGE(double, tarr_sizeF) }
static void tarr_set_rangeS (
  Arrs *this, int64_t ix, Arrs *other, int64_t begin, int64_t end
)  { tarr_SET_RANGE(char*, tarr_sizeS) }
#undef tarr_SET_RANGE
static int64_t tarr_shiftI (Arri *this) {
  if (this->begin == this->end) texc_throw("arr.shift", "Array is empty");
  return tarr_removeI(this, 0);
}
static double tarr_shiftF (Arrf *this) {
  if (this->begin == this->end) texc_throw("arr.shift", "Array is empty");
  return tarr_removeF(this, 0);
}
static char *tarr_shiftS (Arrs *this) {
  if (this->begin == this->end) texc_throw("arr.shift", "Array is empty");
  return tarr_removeS(this, 0);
}
#define tarr_SHUFFLE(Vtype) \
  int rnd_i (int max) { \
    return (int)(((double)rand() / (double)RAND_MAX) * max); \
  } \
  Vtype *begin = this->begin; \
  int size = this->end - begin; \
  Vtype *p = this->end - 1; \
  Vtype *pix; \
  Vtype tmp; \
  while (p > begin) { \
    pix = begin + rnd_i(size--); \
    tmp = *p; \
    *p-- = *pix; \
    *pix = tmp; \
  }
static void tarr_shuffleI (Arri *this) { tarr_SHUFFLE(int64_t) }
static void tarr_shuffleF (Arrf *this) { tarr_SHUFFLE(double) }
static void tarr_shuffleS (Arrs *this) { tarr_SHUFFLE(char*) }
#undef tarr_SHUFFLE

static int tarr_sizeI (Arri *this) { return this->end - this->begin; }
static int tarr_sizeF (Arrf *this) { return this->end - this->begin; }
static int tarr_sizeS (Arrs *this) { return this->end - this->begin; }

#define tarr_SORT(Atype, Vtype, less_fn, take_fn, drop_fn, sort_fn) \
  int sz = this->end - this->begin; \
  if (sz < 2) return; \
  if (sz == 2) { \
    if (less_fn(this->begin[1], this->begin[0])) { \
      Vtype tmp = this->begin[0]; \
      this->begin[0] = this->begin[1]; \
      this->begin[1] = tmp; \
    } \
    return; \
  } \
  if (sz < 17) { \
    Vtype *p = this->begin; \
    Vtype *end = (this->end - 1); \
    while (p < end) { \
      Vtype *q = p + 1; \
      Vtype *qend = (this->end); \
      while (q < qend) { \
        if (less_fn(*q, *p)) { \
          Vtype tmp = *p; \
          *p = *q; \
          *q = tmp; \
        } \
        ++q; \
      } \
      ++p; \
    } \
    return; \
  } \
  int ix = sz / 2; \
  Atype *left = take_fn(this, ix); \
  sort_fn(left); \
  Atype *right = drop_fn(this, ix); \
  sort_fn(right); \
  Vtype *pl = left->begin; \
  Vtype *endl = left->end; \
  Vtype *pr = right->begin; \
  Vtype *endr = right->end; \
  Vtype *p = this->begin; \
  while (pl < endl && pr < endr) { \
    if (less_fn(*pl, *pr)) *p++ = *pl++; \
    else *p++ = *pr++; \
  } \
  while (pl < endl) *p++ = *pl++; \
  while (pr < endr) *p++ = *pr++;
static void tarr_sortIa (Arri *this) {
  int less (int64_t e1, int64_t e2) { return e1 < e2; }
  tarr_SORT(Arri, int64_t, less, tarr_takeI, tarr_dropI, tarr_sortIa)
}
static void tarr_sortId (Arri *this) {
  int less (int64_t e1, int64_t e2) { return e1 > e2; }
  tarr_SORT(Arri, int64_t, less, tarr_takeI, tarr_dropI, tarr_sortId)
}
static void tarr_sortI (Arri *this, int is_ascendant) {
  if (is_ascendant) tarr_sortIa(this);
  else tarr_sortId(this);
}
static void tarr_sortFa(Arrf *this) {
  int less (double e1, double e2) { return e1 < e2; }
  tarr_SORT(Arrf, double, less, tarr_takeF, tarr_dropF, tarr_sortFa)
}
static void tarr_sortFd(Arrf *this) {
  int less (double e1, double e2) { return e1 > e2; }
  tarr_SORT(Arrf, double, less, tarr_takeF, tarr_dropF, tarr_sortFd)
}
static void tarr_sortF (Arrf *this, int is_ascendant) {
  if (is_ascendant) tarr_sortFa(this);
  else tarr_sortFd(this);
}
static void tarr_sortSa (Arrs *this) {
  int less (char * e1, char * e2) { return strcmp(e1, e2) < 0; }
  tarr_SORT(Arrs, char*, less, tarr_takeS, tarr_dropS, tarr_sortSa)
}
static void tarr_sortSd (Arrs *this) {
  int less (char * e1, char * e2) { return strcmp(e1, e2) > 0; }
  tarr_SORT(Arrs, char*, less, tarr_takeS, tarr_dropS, tarr_sortSd)
}
static void tarr_sortS (Arrs *this, int is_ascendant) {
  if (is_ascendant) tarr_sortSa(this);
  else tarr_sortSd(this);
}
#undef tarr_SORT
#define tarr_TAKE(Atype, Vtype, ALLOC_fn) \
  int size = this->end - this->begin; \
  if (n < 0) n = 0; \
  else if (n > size) n = size; \
  Atype *r = MALLOC(Atype); \
  int bf_size = this->bf - this->begin; \
  r->begin = ALLOC_fn(sizeof(Vtype) * bf_size); \
  r->end = r->begin + n; \
  r->bf = r->begin + bf_size; \
  if (n > 0) memcpy(r->begin, this->begin, sizeof(Vtype) * n); \
  return r;
static Arri *tarr_takeI (Arri *this, int64_t n) { tarr_TAKE(Arri, int64_t, ATOMIC) }
static Arrf *tarr_takeF (Arrf *this, int64_t n) { tarr_TAKE(Arrf, double, ATOMIC) }
static Arrs *tarr_takeS (Arrs *this, int64_t n) { tarr_TAKE(Arrs, char*, GC_MALLOC) }
#undef tarr_TAKE
static void tarr_unshiftI (Arri *this, int64_t e) {
  return tarr_insertI(this, 0, e);
}
static void tarr_unshiftF (Arrf *this, double e) {
  return tarr_insertF(this, 0, e);
}
static void tarr_unshiftS (Arrs *this, char *e) {
  return tarr_insertS(this, 0, e);
}
