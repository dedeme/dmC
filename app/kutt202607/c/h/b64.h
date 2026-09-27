
// b64 ---------------------------------

// s -> s
Val __b64_decode (char *pos, Val b64);

// s -> <bytes>
Val __b64_decodeBytes (char *pos, Val b64);

// s -> s
Val __b64_encode (Val s);

// <bytes> -> s
Val __b64_encodeBytes (Val s);

