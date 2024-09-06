static char *cryp_genk (int lg) {
  if (lg <= 0)
    texc_expect("cryp.genK",
      "Value out of range", str_f("%d", lg), "Value < 0");

  char *r = ATOMIC(lg + 1);
  char *p = r + lg;
  *p-- = 0;
  while (lg--) {
    *p-- = *(b64_table + math_rndi(64));
  }
  return r;
}
static char *cryp_key (char *key, int lg) {
  if (!*key)
    texc_expect("cryp.key", "Wrong key", "\"\"", "Value != \"\"");

  char *k0 = str_f(
    "%scodified in irreversibleDeme is good, very good!\n\r8@@", key);
  char *b64 = b64_encode(k0);
  Bytes *k = b64_decode_bytes(b64)->value;

  unsigned char *ka = k->bs;
  size_t lenk = k->length;
  unsigned char sum = 0;
  unsigned char *p = ka;
  for (int i = 0; i < lenk; ++i) sum += *p++;

  size_t lg2 = lg + lenk;
  Bytes *rbs = bytes_new(lg2)->value;
  unsigned char *ra = rbs->bs;
  Bytes *rbs1 = bytes_new(lg2)->value;
  unsigned char *ra1 = rbs1->bs;
  Bytes *rbs2 = bytes_new(lg2)->value;
  unsigned char *ra2 = rbs2->bs;

  size_t ik = 0;
  size_t v1, v2, v3, v4;
  for (size_t i = 0; i < lg2; ++i) {
    v1 = ka[ik];
    v2 = v1 + ka[v1 % lenk];
    v3 = v2 + ka[v2 % lenk];
    v4 = v3 + ka[v3 % lenk];
    sum = sum + i + v4;
    ra1[i] = sum;
    ra2[i] = sum;
    ++ik;
    if (ik == lenk) {
      ik = 0;
    }
  }

  for (size_t i = 0; i < lg2; ++i) {
    v1 = ra2[i];
    v2 = v1 + ra2[v1 % lg2];
    v3 = v2 + ra2[v2 % lg2];
    v4 = v3 + ra2[v3 % lg2];
    sum = sum + v4;
    ra2[i] = sum;
    ra[i] = sum + ra1[i];
  }

  Obj *orbs = obj_new("<bytes>", rbs);
  return str_left(b64_encode_bytes(orbs), lg);
}
static char *cryp_encode (char *k, char *s) {
  if (!*k)
    texc_expect("cryp.encode", "Wrong key", "\"\"", "Value != \"\"");

  char *b64 = b64_encode(s);

  size_t lg = strlen(b64);
  char *k2 = cryp_key(k, lg);

  Bytes *rbs = bytes_new(lg)->value;

  unsigned char *prbs = rbs->bs;
  unsigned char *pk2 = (unsigned char *)k2;
  unsigned char *pb64 = (unsigned char *)b64;
  for(int i = 0; i < lg; ++i) *prbs++ = (*pk2++) + (*pb64++);

  Obj *orbs = obj_new("<bytes>", rbs);
  return b64_encode_bytes(orbs);
}
static char *cryp_decode (char *k, char *c) {
  if (!*k)
    texc_expect("cryp.decode", "Wrong key", "\"\"", "Value != \"\"");

  Bytes *bs = b64_decode_bytes(c)->value;

  size_t lg = bs->length;
  char *k2 = cryp_key(k, lg);

  char *b64 = ATOMIC(lg + 1);

  unsigned char *pbs = bs->bs;
  unsigned char *pk2 = (unsigned char *)k2;
  char *pb64 = b64;
  for(int i = 0; i < lg; ++i) *pb64++ = *pbs++ - *pk2++;
  *pb64 = 0;

  return b64_decode(b64);
}
