
// cryp ---------------------------

// s, s -> s
Val __cryp_decode (Val k, Val c) {
  return (Val) cryp_decode(k.s, c.s);
}

// s, s -> <bytes>
Val __cryp_decodeBytes (Val k, Val c) {
  return (Val)(void *)cryp_decode_bytes(k.s, c.s);
}

// s, s -> s
Val __cryp_encode (Val k, Val tx) {
  return (Val)cryp_encode(k.s, tx.s);
}

// s, <bytes> -> s
Val __cryp_encodeBytes (Val k, Val bs) {
  return (Val)cryp_encode_bytes(k.s, bs.o);
}

// i -> s
Val __cryp_genK (Val lg) {
  return (Val)cryp_genk(lg.i);
}

// s, i -> s
Val __cryp_key (Val s, Val lg) {
  return (Val)cryp_key(s.s, lg.i);
}
