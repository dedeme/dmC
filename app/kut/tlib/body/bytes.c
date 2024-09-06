static Obj *bytes_add (Obj *othis, Obj *oother) {
  texc_check_type("bytes.add", othis, "<bytes>");
  texc_check_type("bytes.add", oother, "<bytes>");
  Bytes *this = othis->value;
  Bytes *other = oother->value;
  int length = other->length;
  if (length) {
    int sum = this->length + length;
    this->bs = GC_REALLOC(this->bs, sum);
    memcpy(this->bs + this->length, other->bs, length);
    this->length = sum;
  }
  return obj_new("<bytes>", this);
}
static Obj *bytes_drop (Obj *othis, int64_t n) {
  texc_check_type("bytes.drop", othis, "<bytes>");
  Bytes *this = othis->value;
  int64_t sz = this->length;
  if (n >= sz) return bytes_new(0);
  if (n < 0) n = 0;
  sz = sz - n;
  Obj *onew = bytes_new(sz);
  Bytes *new = onew->value;
  memcpy(new->bs, this->bs + n, sz);
  return onew;
}
static Obj *bytes_from_arr (Arri *a) {
  int64_t *p = tarr_beginI(a);
  int64_t *end = tarr_endI(a);
  Obj *othis = bytes_new(end - p);
  Bytes *this = othis->value;
  unsigned char *bs = this->bs;
  while (p < end) *bs++ = (unsigned char)*p++;
  return othis;
}
static Obj *bytes_from_bytes(unsigned char *bs, int length) {
  Bytes *this = MALLOC(Bytes);
  this->bs = ATOMIC(length);
  memcpy(this->bs, bs, length);
  this->length = length;
  return obj_new("<bytes>", this);
}
static Obj *bytes_from_str (char *s) {
  Obj *othis = bytes_new(strlen(s));
  Bytes *this = othis->value;
  unsigned char *bs = this->bs;
  while (*s) *bs++ = (unsigned char)*s++;
  return othis;
}
static int64_t bytes_get (Obj *othis, int64_t ix) {
  texc_check_type("bytes.get", othis, "<bytes>");
  Bytes *this = othis->value;
  texc_check_range("bytes.get", 0, this->length - 1, ix);
  return this->bs[ix];
}
static Obj *bytes_new (int64_t length) {
  Bytes *this = MALLOC(Bytes);
  this->bs = ATOMIC(length);
  memset(this->bs, 0, length);
  this->length = length;
  return obj_new("<bytes>", this);
}
static void bytes_set (Obj *othis, int64_t ix, int64_t v) {
  texc_check_type("bytes.get", othis, "<bytes>");
  Bytes *this = othis->value;
  texc_check_range("bytes.get", 0, this->length - 1, ix);
  this->bs[ix] = (unsigned char) v;
}
static int64_t bytes_size (Obj *othis) {
  texc_check_type("bytes.get", othis, "<bytes>");
  Bytes *this = othis->value;
  return this->length;
}
static Obj *bytes_take (Obj *othis, int64_t n) {
  texc_check_type("bytes.take", othis, "<bytes>");
  Bytes *this = othis->value;
  if (n <= 0) return bytes_new(0);
  int64_t sz = this->length;
  if (n > sz) n = sz;
  Obj *onew = bytes_new(n);
  Bytes *new = onew->value;
  memcpy(new->bs, this->bs, n);
  return onew;
}
static Arri *bytes_to_arr (Obj *othis) {
  texc_check_type("bytes.toArr", othis, "<bytes>");
  Bytes *this = othis->value;
  Arri *a = tarr_newi(0, this->length);
  int64_t *p = tarr_beginI(a);
  int64_t *end = tarr_endI(a);
  unsigned char *bs = this->bs;
  while (p < end) *p++ = *bs++;
  return a;

}
static char *bytes_to_str (Obj *othis) {
  texc_check_type("bytes.toStr", othis, "<bytes>");
  Bytes *this = othis->value;
  Buf *b = buf_new_bf(this->length + 1);
  unsigned char *p = this->bs;
  unsigned char *end = this->bs + this->length;
  while (p < end) buf_cadd(b, *p++);
  return str_new(buf_str(b));
}
