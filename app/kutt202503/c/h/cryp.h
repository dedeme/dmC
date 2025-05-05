
// cryp ------------------------------

// s, s -> s
Val __cryp_decode (Val k, Val c);

// s, s -> <bytes>
Val __cryp_decodeBytes (Val k, Val c);

// s, s -> s
Val __cryp_encode (Val k, Val tx);

// s, <bytes> -> s
Val __cryp_encodeBytes (Val k, Val bs);

// i -> s
Val __cryp_genK (Val lg);

// s, i -> s
Val __cryp_key (Val s, Val lg);

