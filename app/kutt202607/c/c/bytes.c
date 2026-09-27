
// bytes -------------------------------

// <bytes>,<bytes> -> <bytes>
Val __bytes_add (Val bs1, Val bs2) {
  bytes_add((Bytes *)bs1.o, (Bytes *)bs2.o);
  return bs1;
}

// <bytes>,n -> <bytes>
Val __bytes_drop (Val bts, Val begin) {
  Bytes *bs = (Bytes *)bts.o;
  int64_t ix = begin.i;
  int64_t sz = bytes_len(bs);
  Bytes *bs2 = sz > ix
    ? ix <= 0
      ? bytes_from_bytes(bytes_bs(bs), sz)
      : bytes_from_bytes(bytes_bs(bs) + ix, sz - ix)
    : bytes_new()
  ;
  return (Val)(void *)bs2;
}

// <bytes>,n -> <bytes>Val bytes_drop (Val bs, Val begin);
// [.i] -> <bytes>
Val __bytes_fromArr (Val a) {
  int64_t sz = __arr_size(a).i;
  Bytes *bs = bytes_new_bf(sz);
  unsigned char *pu = bytes_bs(bs);
  Val *p = a.a->begin;
  Val *end = a.a->end;
  while (p < end) *pu++ = (unsigned char)(*p++).i;
  return (Val)(void *)bs;
}

// s -> <bytes>
Val __bytes_fromStr (Val str) {
  return (Val)(void *)bytes_from_str(str.s);
}

// s,<bytes>,i->i
Val __bytes_get (char *pos, Val bs, Val ix) {
  int64_t sz = __bytes_size(bs).i;
  int64_t i = ix.i;
  if (i < 0 || i >= sz)
    ___built_throw(pos, (Val)str_f("Index %d out of [0,%d]", i, sz - 1));
  return (Val)(int64_t)(bytes_bs((Bytes *)bs.o)[ix.i]);
}

// i -> <bytes>
Val __bytes_new (Val size) {
  int64_t bf = size.i;
  if (bf < 0) bf = 0;
  Bytes *bs = bytes_new_bf(bf);
  unsigned char *p = bytes_bs(bs);
  while (bf) {
    *p++ = 0;
    --bf;
  }
  return (Val)(void *)bs;
}

// s,<bytes>,i,i->()
void __bytes_set (char *pos, Val bs, Val ix, Val byte) {
  int64_t sz = __bytes_size(bs).i;
  int64_t i = ix.i;
  if (i < 0 || i >= sz)
    ___built_throw(pos, (Val)str_f("Index %d out of [0,%d]", i, sz - 1));
  bytes_bs((Bytes *)bs.o)[ix.i] = (unsigned char)byte.i;
}

// <bytes> -> i
Val __bytes_size (Val bs) {
  return (Val)(int64_t)bytes_len((Bytes *)bs.o);
}

// <bytes>,i -> <bytes>
Val __bytes_take (Val bts, Val end) {
  Bytes *bs = (Bytes *)bts.o;
  int64_t ix = end.i;
  int64_t sz = bytes_len(bs);
  Bytes *bs2 = ix <= 0
    ? bytes_new()
    : ix >= sz
      ? bytes_from_bytes(bytes_bs(bs), sz)
      : bytes_from_bytes(bytes_bs(bs), ix)
  ;
  return (Val)(void *)bs2;
}

// <bytes> -> [.i]
Val __bytes_toArr (Val bts) {
  Bytes *bs = (Bytes *)bts.o;
  int sz = bytes_len(bs);
  Val a = __arr_new();
  unsigned char *p = bytes_bs(bs);
  while (sz) {
    __arr_push(a, (Val)(int64_t)*p++);
    --sz;
  }
  return a;
}

// <bytes> -> s
Val __bytes_toStr (Val bs) {
  return (Val)bytes_to_str((Bytes *)bs.o);
}

