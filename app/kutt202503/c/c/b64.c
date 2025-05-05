
// b64 ------------------------------

// s -> s
Val __b64_decode (char *pos, Val b64) {
  char *r;
  TRY {
    r = b64_decode(b64.s);
  } CATCH(e) {
    ___built_throw(pos, (Val)exc_msg(e));
  }_TRY
  return (Val) r;
}

// s -> <bytes>
Val __b64_decodeBytes (char *pos, Val b64) {
  Bytes *r;
  TRY {
    r = b64_decode_bytes(b64.s);
  } CATCH(e) {
    ___built_throw(pos, (Val)exc_msg(e));
  }_TRY
  return (Val) (void *)r;
}

// s -> s
Val __b64_encode (Val s) {
  return (Val)b64_encode(s.s);
}

// <bytes> -> s
Val __b64_encodeBytes (Val bs) {
  return (Val)b64_encode_bytes(bs.o);
}

