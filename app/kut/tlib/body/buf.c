struct buf_Buf{
  char *str;
  int len;
  int bf_size;
};

static Buf *buf_new () {
  return buf_new_bf(151);
}
static Buf *buf_new_bf (int buffer_size) {
  Buf *this = MALLOC(Buf);
  this->bf_size = buffer_size - 1;
  this->str = ATOMIC(buffer_size);
  memset(this->str, 0, buffer_size);
  this->len = 0;
  return this;
}
static char *buf_str (Buf *this) {
  return this->str;
}
static int buf_len (Buf *this) {
  return this->len;
}
static void buf_add_buf (Buf *this, char *data, int length) {
  int ixend = this->len + length;
  if (this->bf_size < ixend) {
      while (this->bf_size < ixend) this->bf_size += this->bf_size;
      int memsize = this->bf_size + 1;
      this->str = GC_REALLOC(this->str, memsize);
      memset(this->str + this->len, 0, memsize - this->len);
  }
  memcpy(this->str + this->len, data, length);
  this->len = ixend;
}
static void buf_add (Buf *this, char *data) {
  buf_add_buf(this, data, strlen(data));
}
static void buf_cadd (Buf *this, char data) {
  buf_add_buf(this, &data, 1);
}
static void buf_reset(Buf *this) {
  memset(this->str, 0, this->len);
  this->len = 0;
}
